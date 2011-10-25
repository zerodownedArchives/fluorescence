
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

#include <ui/manager.hpp>
#include <ui/texture.hpp>
#include <ui/textureprovider.hpp>
#include <ui/animtextureprovider.hpp>
#include <ui/singletextureprovider.hpp>

#include <net/manager.hpp>
#include <net/packets/singleclick.hpp>
#include <net/packets/doubleclick.hpp>
#include <net/packets/pickupitem.hpp>
#include <net/packets/dropitem.hpp>

namespace fluo {
namespace world {

DynamicItem::DynamicItem(Serial serial) : ServerObject(serial, IngameObject::TYPE_DYNAMIC_ITEM), artId_(0), equipped_(false) {
}

boost::shared_ptr<ui::Texture> DynamicItem::getIngameTexture() const {
    if (equipped_) {
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
    if (artId_ != artId) {
        artId_ = artId;
        tileDataInfo_ = data::Manager::getTileDataLoader()->getStaticTileInfo(artId_);
        worldRenderData_.hueInfo_[0u] = tileDataInfo_->partialHue() ? 1.0 : 0.0;
        worldRenderData_.hueInfo_[2u] = tileDataInfo_->translucent() ? 0.8 : 1.0;

        invalidateTextureProvider();
    }

    addToRenderQueue(ui::Manager::getWorldRenderQueue());
}

void DynamicItem::setDirection(unsigned int direction) {
    direction_ = direction;

    if (equipped_ && animTextureProvider_) {
        animTextureProvider_->setDirection(direction_);
        invalidateVertexCoordinates();
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

void DynamicItem::updateVertexCoordinates() {
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

        CL_Rectf rect(px, py, px + texWidth, py + texHeight);

        worldRenderData_.vertexCoordinates_[0] = CL_Vec2f(rect.left, rect.top);
        worldRenderData_.vertexCoordinates_[1] = CL_Vec2f(rect.right, rect.top);
        worldRenderData_.vertexCoordinates_[2] = CL_Vec2f(rect.left, rect.bottom);
        worldRenderData_.vertexCoordinates_[3] = CL_Vec2f(rect.right, rect.top);
        worldRenderData_.vertexCoordinates_[4] = CL_Vec2f(rect.left, rect.bottom);
        worldRenderData_.vertexCoordinates_[5] = CL_Vec2f(rect.right, rect.bottom);
    } else {
        int texWidth = getIngameTexture()->getWidth();
        int texHeight = getIngameTexture()->getHeight();

        int px = (getLocX() - getLocY()) * 22 - texWidth/2 + 22;
        int py = (getLocX() + getLocY()) * 22 - texHeight + 44;
        py -= getLocZ() * 4;

        CL_Rectf rect(px, py, px + texWidth, py + texHeight);

        worldRenderData_.vertexCoordinates_[0] = CL_Vec2f(rect.left, rect.top);
        worldRenderData_.vertexCoordinates_[1] = CL_Vec2f(rect.right, rect.top);
        worldRenderData_.vertexCoordinates_[2] = CL_Vec2f(rect.left, rect.bottom);
        worldRenderData_.vertexCoordinates_[3] = CL_Vec2f(rect.right, rect.top);
        worldRenderData_.vertexCoordinates_[4] = CL_Vec2f(rect.left, rect.bottom);
        worldRenderData_.vertexCoordinates_[5] = CL_Vec2f(rect.right, rect.bottom);
    }
}

void DynamicItem::updateRenderPriority() {
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

        // level 0 x+y
        worldRenderData_.renderPriority_[0] = parent->getLocX() + parent->getLocY();

        // level 1 z
        worldRenderData_.renderPriority_[1] = parent->getLocZ() + 7;

        // level 2 type of object (map behind statics behind dynamics behind mobiles if on same coordinates)
        worldRenderData_.renderPriority_[2] = 40;

        // level 2 layer priority
        unsigned int layerTmp = layer_ - 1;
        if (layerTmp >= layerPriorities[parent->getDirection()].size()) {
            LOG_WARN << "Rendering item with invalid layer " << layer_ << ". Unable to assign render priority" << std::endl;
            layerTmp = 0;
        }
        worldRenderData_.renderPriority_[3] = layerPriorities[parent->getDirection()][layerTmp];

        // level 5 serial
        worldRenderData_.renderPriority_[5] = getSerial();
    } else {
        // level 0 x+y
        worldRenderData_.renderPriority_[0] = getLocX() + getLocY();

        // level 1 z and tiledata flags
        worldRenderData_.renderPriority_[1] = getLocZ();
        if (tileDataInfo_->background() && tileDataInfo_->surface()) {
            worldRenderData_.renderPriority_[1] += 2;
        } else if (tileDataInfo_->background()) {
            worldRenderData_.renderPriority_[1] += 3;
        } else if (tileDataInfo_->surface()) {
            worldRenderData_.renderPriority_[1] += 4;
        } else {
            worldRenderData_.renderPriority_[1] += 6;
        }

        // level 2 type of object (map behind statics behind dynamics behind mobiles if on same coordinates)
        worldRenderData_.renderPriority_[2] = 20;

        // level 3 tiledata value height
        worldRenderData_.renderPriority_[3] = tileDataInfo_->height_;

        // level 4 serial
        worldRenderData_.renderPriority_[5] = getSerial();
    }
}

void DynamicItem::updateTextureProvider() {
    if (equipped_) {
        animTextureProvider_.reset(new ui::AnimTextureProvider(tileDataInfo_->animId_));
        boost::shared_ptr<Mobile> parent = boost::dynamic_pointer_cast<Mobile>(parentObject_.lock());
        setDirection(parent->getDirection());

        gumpTextureProvider_.reset(new ui::SingleTextureProvider(ui::SingleTextureProvider::FROM_GUMPART_MUL, tileDataInfo_->animId_ + 50000));
        LOG_DEBUG << "Gump idx " << tileDataInfo_->animId_ + 50000 << std::endl;
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
    return direction_ < 3;
}

void DynamicItem::playAnim(unsigned int animId) {
    if (equipped_ && animTextureProvider_) {
        animTextureProvider_->setAnimId(animId);
    }
}

void DynamicItem::onAddedToParent() {
    if (parentObject_.lock()->isMobile()) {
        equipped_ = true;
        invalidateTextureProvider();
        invalidateRenderPriority();
    }
}

void DynamicItem::onRemovedFromParent() {
    if (equipped_) {
        equipped_ = false;
        invalidateTextureProvider();
        invalidateRenderPriority();
    }
}

}
}
