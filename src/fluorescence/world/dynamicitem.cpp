/*
 * fluorescence is a free, customizable Ultima Online client.
 * Copyright (C) 2011-2012, http://fluorescence-client.org

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "dynamicitem.hpp"

#include "map.hpp"
#include "statics.hpp"
#include "mobile.hpp"

#include "smoothmovement.hpp"
#include "smoothmovementmanager.hpp"
#include "manager.hpp"

#include <typedefs.hpp>
#include <client.hpp>
#include <misc/config.hpp>

#include <data/manager.hpp>
#include <data/artloader.hpp>
#include <data/tiledataloader.hpp>
#include <data/gumpartloader.hpp>

#include <ui/manager.hpp>
#include <ui/texture.hpp>
#include <ui/textureprovider.hpp>
#include <ui/animtextureprovider.hpp>
#include <ui/singletextureprovider.hpp>
#include <ui/gumpmenu.hpp>
#include <ui/components/containerview.hpp>
#include <ui/render/material.hpp>

#include <net/manager.hpp>
#include <net/packets/09_singleclick.hpp>
#include <net/packets/06_doubleclick.hpp>
#include <net/packets/07_pickupitem.hpp>
#include <net/packets/08_dropitem.hpp>
#include <net/packets/13_equiprequest.hpp>

namespace fluo {
namespace world {

DynamicItem::DynamicItem(Serial serial) : ServerObject(serial, IngameObject::TYPE_DYNAMIC_ITEM),
        artId_(0), tileDataInfo_(nullptr), equipped_(false), spellbookGump_(nullptr), containerView_(nullptr), isSpellbook_(false) {
}

boost::shared_ptr<ui::Texture> DynamicItem::getIngameTexture() const {
    if (equipped_ || !textureProvider_) {
        return animTextureProvider_->getTexture();
    } else {
        return textureProvider_->getTexture();
    }
}

boost::shared_ptr<ui::Texture> DynamicItem::getGumpTexture() const {
    return gumpTextureProvider_->getTexture();
}

unsigned int DynamicItem::getArtId() const {
    return artId_;
}

void DynamicItem::setArtId(unsigned int artId) {
    if (!tileDataInfo_ || artId_ != artId) { // not initialized or other art id
        artId_ = artId;
        tileDataInfo_ = data::Manager::getTileDataLoader()->getStaticTileInfo(artId_);
        worldRenderData_.hueInfo_[0u] = tileDataInfo_->partialHue() ? 1.0 : 0.0;
        worldRenderData_.hueInfo_[2u] = tileDataInfo_->translucent() ? 0.8 : 1.0;

        setIgnored(ui::Manager::isStaticIdIgnored(artId_));
        if (ui::Manager::isStaticIdWater(artId_)) {
            invalidateTextureProvider();
        }
    }
}

void DynamicItem::setDirection(unsigned int direction) {
    if (direction_ != direction) {
        direction_ = direction;

        if (equipped_ && animTextureProvider_) {
            animTextureProvider_->setDirection(direction_);
            invalidateVertexCoordinates();

            forceRepaint();
        }
    }
}

unsigned int DynamicItem::getDirection() const {
    return direction_;
}

void DynamicItem::setAmount(unsigned int amount) {
    if (amount != amount_) {
        amount_ = amount;
    }

    // TODO: display change when stacked
}

unsigned int DynamicItem::getAmount() const {
    return amount_;
}

void DynamicItem::setStackIdOffset(unsigned int offset) {
    if (stackIdOffset_ != offset) {
        stackIdOffset_ = offset;
    }

    // TODO: display change when stacked
}

void DynamicItem::updateVertexCoordinates() {
    CL_Rectf rect;

    if (equipped_) {
        ui::AnimationFrame frame = animTextureProvider_->getCurrentFrame();
        int texWidth = frame.texture_->getWidth();
        int texHeight = frame.texture_->getHeight();


        //int px = (getLocX() - getLocY()) * 22 - texWidth/2 + 22;
        //int py = (getLocX() + getLocY()) * 22 - texHeight + 44;
        //py -= getLocZ() * 4;

        boost::shared_ptr<Mobile> parent = boost::static_pointer_cast<Mobile>(parentObject_.lock());

        int px = (parent->getLocXDraw() - parent->getLocYDraw()) * 22 + 22;
        int py = (parent->getLocXDraw() + parent->getLocYDraw()) * 22 - parent->getLocZDraw() * 4 + 22;
        py = py - frame.centerY_ - texHeight;

        if (isMirrored()) {
            px = px - texWidth + frame.centerX_;
        } else {
            px -= frame.centerX_;
        }

        rect = CL_Rectf(px, py, px + texWidth, py + texHeight);
    } else {
        int texWidth = getIngameTexture()->getWidth();
        int texHeight = getIngameTexture()->getHeight();

        int px = (getLocXDraw() - getLocYDraw()) * 22 - texWidth/2 + 22;
        int py = (getLocXDraw() + getLocYDraw()) * 22 - texHeight + 44;
        py -= getLocZDraw() * 4;

        rect = CL_Rectf(px, py, px + texWidth, py + texHeight);
    }

    worldRenderData_.setVertexCoordinates(rect);
}

void DynamicItem::updateRenderDepth() {
    static bool initialized = false;
    static std::vector<int> layerPriorities[8];
    if (!initialized) {
        Config& cfg = Client::getSingleton()->getConfig();
        cfg["/fluo/ui/layer-priorities@north"].toIntList(layerPriorities[Direction::N]);
        cfg["/fluo/ui/layer-priorities@northeast"].toIntList(layerPriorities[Direction::NE]);
        cfg["/fluo/ui/layer-priorities@east"].toIntList(layerPriorities[Direction::E]);
        cfg["/fluo/ui/layer-priorities@southeast"].toIntList(layerPriorities[Direction::SE]);
        cfg["/fluo/ui/layer-priorities@south"].toIntList(layerPriorities[Direction::S]);
        cfg["/fluo/ui/layer-priorities@southwest"].toIntList(layerPriorities[Direction::SW]);
        cfg["/fluo/ui/layer-priorities@west"].toIntList(layerPriorities[Direction::W]);
        cfg["/fluo/ui/layer-priorities@northwest"].toIntList(layerPriorities[Direction::NW]);

        initialized = true;
    }

    if (equipped_) {
        boost::shared_ptr<Mobile> parent = boost::static_pointer_cast<Mobile>(parentObject_.lock());

        unsigned int layerTmp = layer_ - 1;
        if (layerTmp >= layerPriorities[parent->getDirection()].size()) {
            if (layerTmp > 0x1D) {
                // up to 0x1d are some special layers, like mounts, bank box, etc
                LOG_WARN << "Rendering item with invalid layer " << layer_ << ". Unable to assign render priority" << std::endl;
            }
            layerTmp = 0;
        }

        int8_t z = parent->getLocZGame() + 7;

        uint8_t order = (equipped_ && getLayer() == Layer::MOUNT) ? 29 : 40;
        worldRenderData_.setRenderDepth(parent->getLocXGame(), parent->getLocYGame(), z, order, layerPriorities[parent->getDirection()][layerTmp], getSerial() & 0xFF);
    } else {
        int8_t z = getLocZGame();
        if (tileDataInfo_->background() && tileDataInfo_->surface()) {
            z += 4;
        } else if (tileDataInfo_->background()) {
            z += 2;
        } else if (tileDataInfo_->surface()) {
            z += 5;
        } else {
            z += 6;
        }

        worldRenderData_.setRenderDepth(getLocXGame(), getLocYGame(), z, 20, tileDataInfo_->height_, getSerial() & 0xFF);
    }
}

void DynamicItem::updateTextureProvider() {
    if (equipped_) {
        boost::shared_ptr<Mobile> parent = boost::static_pointer_cast<Mobile>(parentObject_.lock());

        unsigned int animId;
        unsigned int idleAnim = parent->getIdleAnim();

        if (layer_ == Layer::MOUNT) {
            animId = data::Manager::getMountDef(artId_).animId_;
            animType_ = data::Manager::getAnimType(animId);
            if (animType_ == AnimType::HIGH_DETAIL) {
                idleAnim = 1;
            } else {
                idleAnim = 2;
            }
        } else {
            animId = tileDataInfo_->animId_;
        }

        animTextureProvider_.reset(new ui::AnimTextureProvider(animId, idleAnim));
        animTextureProvider_->setDirection(getDirection());

        // TODO: load this on demand
        unsigned int gumpId = data::Manager::getGumpIdForItem(artId_, parent->getBodyId());
        gumpTextureProvider_.reset(new ui::SingleTextureProvider(data::TextureSource::GUMPART, gumpId));
        //LOG_DEBUG << "Gump idx " << gumpId << std::endl;
    } else {
        textureProvider_ = data::Manager::getItemTextureProvider(artId_);

        animTextureProvider_.reset(); // remove anim tex provider if not equipped
        gumpTextureProvider_.reset();
    }
}

bool DynamicItem::updateAnimation(unsigned int elapsedMillis) {
    if (equipped_) {
        return animTextureProvider_->update(elapsedMillis);
    } else {
        return textureProvider_->update(elapsedMillis) || materialInfo_->constantRepaint_;
    }
}

const data::StaticTileInfo* DynamicItem::getTileDataInfo() const {
    return tileDataInfo_;
}

void DynamicItem::onClick() {
    LOG_INFO << "Clicked dynamic, id=" << std::hex << getArtId() << std::dec << " loc=(" << getLocXGame() << "/" << getLocYGame() << "/" <<
            getLocZGame() << ") name=" << tileDataInfo_->name_ << " equipped=" << equipped_ << std::endl;
    //LOG_INFO << "impassable=" << tileDataInfo_->impassable() << " surface=" << tileDataInfo_->surface() << " bridge=" << tileDataInfo_->bridge() << " height=" << (unsigned int)tileDataInfo_->height_ << std::endl;

    //printRenderDepth();

    //net::packets::SingleClick pkt(getSerial());
    //net::Manager::getSingleton()->send(pkt);
}

void DynamicItem::onDoubleClick() {
    LOG_INFO << "Double clicked dynamic, id=" << std::hex << getArtId() << std::dec << " loc=(" << getLocXGame() << "/" << getLocYGame() << "/" <<
            getLocZGame() << ") name=" << tileDataInfo_->name_ << std::endl;

    net::packets::DoubleClick pkt(getSerial());
    net::Manager::getSingleton()->send(pkt);
}

void DynamicItem::onStartDrag(const CL_Point& mousePos) {
    net::packets::PickUpItem pkt(this, getAmount());
    net::Manager::getSingleton()->send(pkt);
}

void DynamicItem::onDraggedOnto(boost::shared_ptr<IngameObject> obj, int locX, int locY) {
    if (obj->isMap() || obj->isStaticItem()) {
        net::packets::DropItem pkt(this, obj->getLocXGame(), obj->getLocYGame(), obj->getLocZGame());
        net::Manager::getSingleton()->send(pkt);
    } else if (obj->isDynamicItem()) {
        boost::shared_ptr<ServerObject> dItem = boost::dynamic_pointer_cast<ServerObject>(obj);
        if (locX != -1 || locY != -1) {
            // drop on container position
            net::packets::DropItem pkt(this, locX, locY, 0, dItem);
            net::Manager::getSingleton()->send(pkt);
        } else {
            net::packets::DropItem pkt(this, dItem);
            net::Manager::getSingleton()->send(pkt);
        }
    } else if (obj->isMobile()) {
        if (locX != -1 || locY != -1) {
            // paperdoll
            boost::shared_ptr<Mobile> mob = boost::dynamic_pointer_cast<Mobile>(obj);
            net::packets::EquipRequest pkt(this->getSerial(), mob->getSerial());
            net::Manager::getSingleton()->send(pkt);
        } else {
            boost::shared_ptr<ServerObject> mob = boost::dynamic_pointer_cast<ServerObject>(obj);
            net::packets::DropItem pkt(this, mob);
            net::Manager::getSingleton()->send(pkt);
        }
    }
}

void DynamicItem::setLayer(unsigned int layer) {
    layer_ = layer;
}

unsigned int DynamicItem::getLayer() const {
    return layer_;
}

bool DynamicItem::isMirrored() const {
    return equipped_ && direction_ < 3;
}

void DynamicItem::animate(unsigned int animId, unsigned int delay, unsigned int repeatMode) {
    if (equipped_ && animTextureProvider_) {
        if (getLayer() == Layer::MOUNT) {
            // translate all anim ids to either walk, run or mount
            if (animId == 24) {
                // running
                animId = (animType_ == AnimType::HIGH_DETAIL ? 0 : 1);
            } else if (animId == 23) {
                // walking
                animId = 1;
            } else {
                // no special anims for other actions
                animId = animTextureProvider_->getDefaultAnimId();
            }
        }

        animTextureProvider_->setAnimId(animId);
        animTextureProvider_->setRepeatMode(repeatMode);
        animTextureProvider_->setDelay(delay);
    }
}

void DynamicItem::setIdleAnim(unsigned int animId) {
    if (equipped_ && animTextureProvider_ && getLayer() != Layer::MOUNT) {
        // idle anim does not change for mounts
        animTextureProvider_->setDefaultAnimId(animId);
    }
}

void DynamicItem::onAddedToParent() {
    if (parentObject_.lock()->isMobile()) {
        equipped_ = true;
        invalidateTextureProvider();
        invalidateRenderDepth();
    }
}

void DynamicItem::onRemovedFromParent() {
    if (equipped_) {
        equipped_ = false;
        invalidateTextureProvider();
        invalidateRenderDepth();
    }
}

void DynamicItem::openContainerGump(unsigned int gumpId) {
    if (gumpId == 0xFFFF || isSpellbook()) {
        // spellbook
        // do nothing, open on spellbook content packet
    } else if (spellbookGump_) {
        spellbookGump_->bring_to_front();
    } else if (containerView_) {
        containerView_->getGumpMenu()->bring_to_front();
    } else {
        boost::python::dict args;
        boost::shared_ptr<DynamicItem> sharedThis = boost::dynamic_pointer_cast<DynamicItem>(shared_from_this());
        args["item"] = sharedThis;
        args["background"] = gumpId;
        ui::Manager::getSingleton()->openPythonGump("container", args);

        // opening the container gump should set the containerView_ member
        if (containerView_) {
            std::list<boost::shared_ptr<IngameObject> >::iterator iter = childObjects_.begin();
            std::list<boost::shared_ptr<IngameObject> >::iterator end = childObjects_.end();

            for (; iter != end; ++iter) {
                containerView_->addObject(*iter);
            }
        }
    }
}

void DynamicItem::onContainerGumpClosed() {
    containerView_ = nullptr;
}

void DynamicItem::onChildObjectAdded(const boost::shared_ptr<IngameObject>& obj) {
    if (containerView_) {
        containerView_->addObject(obj);
    }
}

void DynamicItem::onBeforeChildObjectRemoved(const boost::shared_ptr<IngameObject>& obj) {
    if (containerView_) {
        containerView_->removeObject(obj);
    }
}

bool DynamicItem::isSpellbook() const {
    return isSpellbook_;
}

void DynamicItem::setSpellbook(unsigned int scrollOffset, const uint8_t* spellBits) {
    spellbookScrollOffset_ = scrollOffset;
    for (unsigned int i = 0; i < 8; ++i) {
        spellbookSpellBits_[i] = spellBits[i];
    }
    isSpellbook_ = true;

    if (spellbookGump_) {
        spellbookGump_->bring_to_front();
    } else {
        boost::shared_ptr<DynamicItem> sharedThis = boost::dynamic_pointer_cast<DynamicItem>(shared_from_this());
        ui::GumpMenus::openSpellbook(sharedThis);
        // spellbookGump_ should be set by the python gump menu
        if (spellbookGump_) {
            spellbookGump_->setCloseCallback(boost::bind(&DynamicItem::spellbookClosedCallback, this));
        }
    }
}

uint8_t DynamicItem::getSpellbookSpellBits(unsigned int byteIndex) const {
    if (byteIndex > 7) {
        LOG_ERROR << "Trying to access spellbits with index " << byteIndex << std::endl;
        byteIndex = 0;
    }

    return spellbookSpellBits_[byteIndex];
}

unsigned int DynamicItem::getSpellbookScrollOffset() const {
    return spellbookScrollOffset_;
}

bool DynamicItem::spellbookClosedCallback() {
    spellbookGump_ = nullptr;
    return false;
}

void DynamicItem::setContainerView(ui::components::ContainerView* view) {
    containerView_ = view;
}

void DynamicItem::setSpellbookGump(ui::GumpMenu* gump) {
    spellbookGump_ = gump;
}

void DynamicItem::onDelete() {
    if (containerView_) {
        containerView_->getGumpMenu()->close();
    }

    if (spellbookGump_) {
        spellbookGump_->close();
    }

    ServerObject::onDelete();
}

}
}
