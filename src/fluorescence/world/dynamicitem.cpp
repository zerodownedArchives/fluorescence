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

#include <net/manager.hpp>
#include <net/packets/singleclick.hpp>
#include <net/packets/doubleclick.hpp>
#include <net/packets/pickupitem.hpp>
#include <net/packets/dropitem.hpp>

namespace fluo {
namespace world {

DynamicItem::DynamicItem(Serial serial) : ServerObject(serial, IngameObject::TYPE_DYNAMIC_ITEM), 
        artId_(0), tileDataInfo_(NULL), equipped_(false), containerGump_(NULL) {
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

        invalidateTextureProvider();
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

        boost::shared_ptr<Mobile> parent = boost::dynamic_pointer_cast<Mobile>(parentObject_.lock());

        int px = (parent->getLocX() - parent->getLocY()) * 22 + 22;
        int py = (parent->getLocX() + parent->getLocY()) * 22 - parent->getLocZ() * 4 + 22;
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

        int px = (getLocX() - getLocY()) * 22 - texWidth/2 + 22;
        int py = (getLocX() + getLocY()) * 22 - texHeight + 44;
        py -= getLocZ() * 4;

        rect = CL_Rectf(px, py, px + texWidth, py + texHeight);
    }

    worldRenderData_.setVertexCoordinates(rect);
}

void DynamicItem::updateRenderDepth() {
    static bool initialized = false;
    static std::vector<int> layerPriorities[8];
    if (!initialized) {
        Config& cfg = Client::getSingleton()->getConfig();
        layerPriorities[Direction::N] = cfg["/fluo/ui/layer-priorities@north"].asIntList();
        layerPriorities[Direction::NE] = cfg["/fluo/ui/layer-priorities@northeast"].asIntList();
        layerPriorities[Direction::E] = cfg["/fluo/ui/layer-priorities@east"].asIntList();
        layerPriorities[Direction::SE] = cfg["/fluo/ui/layer-priorities@southeast"].asIntList();
        layerPriorities[Direction::S] = cfg["/fluo/ui/layer-priorities@south"].asIntList();
        layerPriorities[Direction::SW] = cfg["/fluo/ui/layer-priorities@southwest"].asIntList();
        layerPriorities[Direction::W] = cfg["/fluo/ui/layer-priorities@west"].asIntList();
        layerPriorities[Direction::NW] = cfg["/fluo/ui/layer-priorities@northwest"].asIntList();

        initialized = true;
    }

    if (equipped_) {
        boost::shared_ptr<Mobile> parent = boost::dynamic_pointer_cast<Mobile>(parentObject_.lock());

        unsigned int layerTmp = layer_ - 1;
        if (layerTmp >= layerPriorities[parent->getDirection()].size()) {
            LOG_WARN << "Rendering item with invalid layer " << layer_ << ". Unable to assign render priority" << std::endl;
            layerTmp = 0;
        }

        uint16_t xy = ceilf(parent->getLocX()) + ceilf(parent->getLocY());
        int8_t z = ceilf(parent->getLocZ()) + 7;

        worldRenderData_.setRenderDepth(xy, z, 40, layerPriorities[parent->getDirection()][layerTmp], getSerial() & 0xFF);
    } else {
        uint16_t xy = ceilf(getLocX()) + ceilf(getLocY());

        int8_t z = ceilf(getLocZ());
        if (tileDataInfo_->background() && tileDataInfo_->surface()) {
            z += 4;
        } else if (tileDataInfo_->background()) {
            z += 2;
        } else if (tileDataInfo_->surface()) {
            z += 5;
        } else {
            z += 6;
        }

        worldRenderData_.setRenderDepth(xy, z, 20, tileDataInfo_->height_, getSerial() & 0xFF);
    }
}

void DynamicItem::updateTextureProvider() {
    if (equipped_) {
        animTextureProvider_.reset(new ui::AnimTextureProvider(tileDataInfo_->animId_));
        boost::shared_ptr<Mobile> parent = boost::dynamic_pointer_cast<Mobile>(parentObject_.lock());
        animTextureProvider_->setDirection(getDirection());

        unsigned int gumpId = data::Manager::getGumpIdForItem(artId_, parent->getBodyId());

        gumpTextureProvider_.reset(new ui::SingleTextureProvider(ui::SingleTextureProvider::FROM_GUMPART_MUL, gumpId));
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
        return textureProvider_->update(elapsedMillis);
    }
}

const data::StaticTileInfo* DynamicItem::getTileDataInfo() const {
    return tileDataInfo_;
}

void DynamicItem::onClick() {
    LOG_INFO << "Clicked dynamic, id=" << std::hex << getArtId() << std::dec << " loc=(" << getLocX() << "/" << getLocY() << "/" <<
            getLocZ() << ") name=" << tileDataInfo_->name_ << " equipped=" << equipped_ << std::endl;

    printRenderDepth();

    net::packets::SingleClick pkt(getSerial());
    net::Manager::getSingleton()->send(pkt);
}

void DynamicItem::onDoubleClick() {
    LOG_INFO << "Double clicked dynamic, id=" << std::hex << getArtId() << std::dec << " loc=(" << getLocX() << "/" << getLocY() << "/" <<
            getLocZ() << ") name=" << tileDataInfo_->name_ << std::endl;

    net::packets::DoubleClick pkt(getSerial());
    net::Manager::getSingleton()->send(pkt);
}

void DynamicItem::onStartDrag(const CL_Point& mousePos) {
    net::packets::PickUpItem pkt(this, getAmount());
    net::Manager::getSingleton()->send(pkt);
}

void DynamicItem::onDraggedOnto(boost::shared_ptr<IngameObject> obj) {
    const IngameObject* rawPtr = obj.get();

    const MapTile* mapTile = dynamic_cast<const MapTile*>(rawPtr);
    if (mapTile) {
        net::packets::DropItem pkt(this, mapTile->getLocX(), mapTile->getLocY(), mapTile->getLocZ());
        net::Manager::getSingleton()->send(pkt);
        return;
    }

    const StaticItem* sItem = dynamic_cast<const StaticItem*>(rawPtr);
    if (sItem) {
        net::packets::DropItem pkt(this, sItem->getLocX(), sItem->getLocY(), sItem->getLocZ());
        net::Manager::getSingleton()->send(pkt);
        return;
    }

    // TODO
    const DynamicItem* dItem = dynamic_cast<const DynamicItem*>(rawPtr);
    if (dItem) {
    }

    const Mobile* mobile = dynamic_cast<const Mobile*>(rawPtr);
    if (mobile) {
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
        animTextureProvider_->setAnimId(animId);
        animTextureProvider_->setRepeatMode(repeatMode);
        animTextureProvider_->setDelay(delay);
    }
}

void DynamicItem::stopAnim() {
    if (equipped_ && animTextureProvider_) {
        animTextureProvider_->setIdleAnim();
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
    if (containerGump_) {
        containerGump_->bring_to_front();
    } else {
        containerGump_ = ui::Manager::getSingleton()->openXmlGump("container");
    }

    ui::components::ContainerView* contView = dynamic_cast<ui::components::ContainerView*>(containerGump_->get_named_item("container"));
    if (contView) {
        contView->setBackgroundGumpId(gumpId);
        boost::shared_ptr<DynamicItem> dynSelf = boost::dynamic_pointer_cast<DynamicItem>(shared_from_this());
        contView->setContainerObject(dynSelf);
    } else {
        LOG_ERROR << "Unable to find container component in container gump" << std::endl;
        return;
    }

    std::list<boost::shared_ptr<IngameObject> >::iterator iter = childObjects_.begin();
    std::list<boost::shared_ptr<IngameObject> >::iterator end = childObjects_.end();

    for (; iter != end; ++iter) {
        contView->addObject(*iter);
    }
}

void DynamicItem::onContainerGumpClosed() {
    containerGump_ = NULL;
}

void DynamicItem::onChildObjectAdded(boost::shared_ptr<IngameObject> obj) {
    if (containerGump_) {
        ui::components::ContainerView* contView = dynamic_cast<ui::components::ContainerView*>(containerGump_->get_named_item("container"));
        if (contView) {
            contView->addObject(obj);
        } else {
            LOG_ERROR << "Unable to find container component in container gump" << std::endl;
        }
    }
}

void DynamicItem::onChildObjectRemoved(boost::shared_ptr<IngameObject> obj) {
    if (containerGump_) {
        ui::components::ContainerView* contView = dynamic_cast<ui::components::ContainerView*>(containerGump_->get_named_item("container"));
        if (contView) {
            contView->removeObject(obj);
        } else {
            LOG_ERROR << "Unable to find container component in container gump" << std::endl;
        }
    }
}

}
}
