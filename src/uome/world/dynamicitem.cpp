
#include "dynamicitem.hpp"

#include <data/manager.hpp>
#include <data/artloader.hpp>
#include <data/tiledataloader.hpp>

#include <ui/texture.hpp>

#include <net/manager.hpp>
#include <net/packets/singleclick.hpp>
#include <net/packets/doubleclick.hpp>

namespace uome {
namespace world {

DynamicItem::DynamicItem(Serial serial) : ServerObject(serial), artId_(0) {
}

boost::shared_ptr<ui::Texture> DynamicItem::getIngameTexture() const {
    return textureProvider_->getTexture();
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

    // TODO: what do we need this for?
}

void DynamicItem::setAmount(unsigned int amount) {
    if (amount != amount_) {
        amount_ = amount;
    }

    // TODO: display change when stacked
}

void DynamicItem::setStackIdOffset(unsigned int offset) {
    if (stackIdOffset_ != offset) {
        stackIdOffset_ = offset;
    }

    // TODO: display change when stacked
}

void DynamicItem::setHue(unsigned int hue) {
    hue_ = hue;
    hueInfo_[1] = hue_;
}

void DynamicItem::updateVertexCoordinates() {
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

void DynamicItem::updateRenderPriority() {
    // render prio
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

    // level 2 serial
    renderPriority_[2] = getSerial();
}

void DynamicItem::updateTextureProvider() {
    textureProvider_ = data::Manager::getItemTextureProvider(artId_);
}

bool DynamicItem::updateAnimation(unsigned int elapsedMillis) {
    return textureProvider_->update(elapsedMillis);
}

const data::StaticTileInfo* DynamicItem::getTileDataInfo() const {
    return tileDataInfo_;
}

void DynamicItem::onClick() {
    LOG_INFO << "Clicked dynamic, id=" << std::hex << getArtId() << std::dec << " loc=(" << getLocX() << "/" << getLocY() << "/" <<
            getLocZ() << ") name=" << tileDataInfo_->name_ << std::endl;

    net::packets::SingleClick pkt(getSerial());
    net::Manager::getSingleton()->send(pkt);
}

void DynamicItem::onDoubleClick() {
    LOG_INFO << "Double clicked dynamic, id=" << std::hex << getArtId() << std::dec << " loc=(" << getLocX() << "/" << getLocY() << "/" <<
            getLocZ() << ") name=" << tileDataInfo_->name_ << std::endl;

    net::packets::DoubleClick pkt(getSerial());
    net::Manager::getSingleton()->send(pkt);
}



}
}
