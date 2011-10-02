
#include "dynamicitem.hpp"

#include "map.hpp"
#include "statics.hpp"
#include "mobile.hpp"

#include <typedefs.hpp>
#include <client.hpp>
#include <misc/config.hpp>

#include <data/manager.hpp>
#include <data/artloader.hpp>
#include <data/tiledataloader.hpp>
#include <data/huesloader.hpp>

#include <ui/texture.hpp>
#include <ui/textureprovider.hpp>
#include <ui/animtextureprovider.hpp>

#include <net/manager.hpp>
#include <net/packets/singleclick.hpp>
#include <net/packets/doubleclick.hpp>
#include <net/packets/pickupitem.hpp>
#include <net/packets/dropitem.hpp>

namespace uome {
namespace world {

DynamicItem::DynamicItem(Serial serial) : ServerObject(serial), artId_(0), equipped_(false), parentMobile_(NULL) {
}

boost::shared_ptr<ui::Texture> DynamicItem::getIngameTexture() const {
    if (equipped_) {
        return animTextureProvider_->getTexture();
    } else {
        return textureProvider_->getTexture();
    }
}

unsigned int DynamicItem::getArtId() const {
    return artId_;
}

void DynamicItem::setArtId(unsigned int artId) {
    if (artId_ != artId) {
        artId_ = artId;
        tileDataInfo_ = data::Manager::getTileDataLoader()->getStaticTileInfo(artId_);
        hueInfo_[0] = tileDataInfo_->partialHue() ? 1.0 : 0.0;

        invalidateRenderData(true);
    }

    addToRenderQueue();
}

void DynamicItem::setDirection(unsigned int direction) {
    direction_ = direction;

    if (equipped_ && animTextureProvider_) {
        animTextureProvider_->setDirection(direction_);
    }
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

void DynamicItem::setHue(unsigned int hue) {
    hue_ = hue;
    hueInfo_[1] = data::Manager::getHuesLoader()->translateHue(hue_);
}

void DynamicItem::updateVertexCoordinates() {
    if (equipped_) {
        ui::AnimationFrame frame = animTextureProvider_->getCurrentFrame();
        int texWidth = frame.texture_->getWidth();
        int texHeight = frame.texture_->getHeight();


        //int px = (getLocX() - getLocY()) * 22 - texWidth/2 + 22;
        //int py = (getLocX() + getLocY()) * 22 - texHeight + 44;
        //py -= getLocZ() * 4;

        int px = (parentMobile_->getLocX() - parentMobile_->getLocY()) * 22 + 22;
        int py = (parentMobile_->getLocX() + parentMobile_->getLocY()) * 22 - parentMobile_->getLocZ() * 4 + 22;
        py = py - frame.centerY_ - texHeight;

        if (isMirrored()) {
            px = px - texWidth + frame.centerX_;
        } else {
            px -= frame.centerX_;
        }

        CL_Rectf rect(px, py, px + texWidth, py + texHeight);

        vertexCoordinates_[0] = CL_Vec2f(rect.left, rect.top);
        vertexCoordinates_[1] = CL_Vec2f(rect.right, rect.top);
        vertexCoordinates_[2] = CL_Vec2f(rect.left, rect.bottom);
        vertexCoordinates_[3] = CL_Vec2f(rect.right, rect.top);
        vertexCoordinates_[4] = CL_Vec2f(rect.left, rect.bottom);
        vertexCoordinates_[5] = CL_Vec2f(rect.right, rect.bottom);
    } else {
        int texWidth = getIngameTexture()->getWidth();
        int texHeight = getIngameTexture()->getHeight();

        int px = (getLocX() - getLocY()) * 22 - texWidth/2 + 22;
        int py = (getLocX() + getLocY()) * 22 - texHeight + 44;
        py -= getLocZ() * 4;

        CL_Rectf rect(px, py, px + texWidth, py + texHeight);

        vertexCoordinates_[0] = CL_Vec2f(rect.left, rect.top);
        vertexCoordinates_[1] = CL_Vec2f(rect.right, rect.top);
        vertexCoordinates_[2] = CL_Vec2f(rect.left, rect.bottom);
        vertexCoordinates_[3] = CL_Vec2f(rect.right, rect.top);
        vertexCoordinates_[4] = CL_Vec2f(rect.left, rect.bottom);
        vertexCoordinates_[5] = CL_Vec2f(rect.right, rect.bottom);
    }
}

void DynamicItem::updateRenderPriority() {
    static bool initialized = false;
    static std::vector<int> layerPriorities[8];
    if (!initialized) {
        Config& cfg = Client::getSingleton()->getConfig();
        layerPriorities[Direction::N] = cfg["/uome/ui/layer-priorities@north"].asIntList();
        layerPriorities[Direction::NE] = cfg["/uome/ui/layer-priorities@northeast"].asIntList();
        layerPriorities[Direction::E] = cfg["/uome/ui/layer-priorities@east"].asIntList();
        layerPriorities[Direction::SE] = cfg["/uome/ui/layer-priorities@southeast"].asIntList();
        layerPriorities[Direction::S] = cfg["/uome/ui/layer-priorities@south"].asIntList();
        layerPriorities[Direction::SW] = cfg["/uome/ui/layer-priorities@southwest"].asIntList();
        layerPriorities[Direction::W] = cfg["/uome/ui/layer-priorities@west"].asIntList();
        layerPriorities[Direction::NW] = cfg["/uome/ui/layer-priorities@northwest"].asIntList();

        initialized = true;
    }

    if (equipped_) {
        // level 0 x+y
        renderPriority_[0] = parentMobile_->getLocX() + parentMobile_->getLocY();

        // level 1 z
        renderPriority_[1] = parentMobile_->getLocZ() + 7;

        // level 2 type of object (map behind statics behind dynamics behind mobiles if on same coordinates)
        renderPriority_[2] = 40;

        // level 2 layer priority
        renderPriority_[3] = layerPriorities[parentMobile_->getDirection()][layer_ - 1];

        // level 5 serial
        renderPriority_[5] = getSerial();
    } else {
        // level 0 x+y
        renderPriority_[0] = getLocX() + getLocY();

        // level 1 z and tiledata flags
        renderPriority_[1] = getLocZ();
        if (tileDataInfo_->background() && tileDataInfo_->surface()) {
            renderPriority_[1] += 2;
        } else if (tileDataInfo_->background()) {
            renderPriority_[1] += 3;
        } else if (tileDataInfo_->surface()) {
            renderPriority_[1] += 4;
        } else {
            renderPriority_[1] += 6;
        }

        // level 2 type of object (map behind statics behind dynamics behind mobiles if on same coordinates)
        renderPriority_[2] = 20;

        // level 3 tiledata value height
        renderPriority_[3] = tileDataInfo_->height_;

        // level 4 if hue is set => higher value
        renderPriority_[4] = (hue_ != 0) ? 1 : 0;

        // level 5 serial
        renderPriority_[5] = getSerial();
    }
}

void DynamicItem::updateTextureProvider() {
    if (equipped_) {
        animTextureProvider_.reset(new ui::AnimTextureProvider(tileDataInfo_->animId_));
        setDirection(parentMobile_->getDirection());
    } else {
        textureProvider_ = data::Manager::getItemTextureProvider(artId_);

        animTextureProvider_.reset(); // remove anim tex provider if not equipped
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
            getLocZ() << ") name=" << tileDataInfo_->name_ << std::endl;

    printRenderPriority();

    net::packets::SingleClick pkt(getSerial());
    net::Manager::getSingleton()->send(pkt);
}

void DynamicItem::onDoubleClick() {
    LOG_INFO << "Double clicked dynamic, id=" << std::hex << getArtId() << std::dec << " loc=(" << getLocX() << "/" << getLocY() << "/" <<
            getLocZ() << ") name=" << tileDataInfo_->name_ << std::endl;

    net::packets::DoubleClick pkt(getSerial());
    net::Manager::getSingleton()->send(pkt);
}

void DynamicItem::onStartDrag() {
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

void DynamicItem::equipOn(Mobile* mob) {
    parentMobile_ = mob;
    equipped_ = true;
    invalidateRenderData(true);
}

void DynamicItem::unequip() {
    equipped_ = false;
    parentMobile_ = NULL;
    invalidateRenderData(true);
}

void DynamicItem::setLayer(unsigned int layer) {
    layer_ = layer;
}

bool DynamicItem::isMirrored() const {
    return direction_ < 3;
}

void DynamicItem::onDelete(boost::shared_ptr<DynamicItem> sharedThis) {
    if (equipped_) {
        parentMobile_->unequip(sharedThis);
    }
}

IngameObject* DynamicItem::getTopParent() {
    if (equipped_) {
        return parentMobile_;
    } else {
        return this;
    }
}

void DynamicItem::playAnim(unsigned int animId) {
    if (equipped_) {
        animTextureProvider_->setAnimId(animId);
    }
}

}
}
