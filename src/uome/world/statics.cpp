
#include "statics.hpp"

#include <data/manager.hpp>
#include <data/artloader.hpp>
#include <data/tiledataloader.hpp>

#include <ui/texture.hpp>

namespace uome {
namespace world {

boost::shared_ptr<ui::Texture> StaticItem::getIngameTexture() const {
    return textureProvider_->getTexture();
}

unsigned int StaticItem::getArtId() const {
    return artId_;
}

void StaticItem::set(int locX, int locY, int locZ, unsigned int artId, unsigned int hue) {
    artId_ = artId;
    hue_ = hue;
    tileDataInfo_ = data::Manager::getTileDataLoader()->getStaticTileInfo(artId_);

    setLocation(locX, locY, locZ);

    addToRenderQueue();
}

void StaticItem::updateVertexCoordinates() {
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

void StaticItem::updateRenderPriority() {
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

    // level 2 tiledata value height
    renderPriority_[2] = tileDataInfo_->height_;

    // level 3 if hue is set => higher value
    renderPriority_[3] = (hue_ != 0) ? 1 : 0;

    // level 4 index in statics file
    renderPriority_[4] = indexInBlock_;
}

void StaticItem::updateTextureProvider() {
    textureProvider_ = data::Manager::getItemTextureProvider(artId_);
}

bool StaticItem::updateAnimation(unsigned int elapsedMillis) {
    return textureProvider_->update(elapsedMillis);
}

const data::StaticTileInfo* StaticItem::getTileDataInfo() const {
    return tileDataInfo_;
}

void StaticItem::onClick() {
    LOGARG_INFO(LOGTYPE_INPUT, "Clicked static, id=%x loc=(%i/%i/%i) name=%S",
                getArtId(), getLocX(), getLocY(), getLocZ(), const_cast<data::StaticTileInfo*>(getTileDataInfo())->name_.getTerminatedBuffer());
}


std::list<boost::shared_ptr<StaticItem> >& StaticBlock::getItemList() {
    return itemList_;
}


}
}
