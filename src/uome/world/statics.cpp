
#include "statics.hpp"

#include <data/manager.hpp>
#include <data/artloader.hpp>
#include <data/tiledataloader.hpp>
#include <data/huesloader.hpp>

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

    hueInfo_[0] = tileDataInfo_->partialHue() ? 1.0 : 0.0;
    hueInfo_[1] = data::Manager::getHuesLoader()->translateHue(hue_);

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

    // level 2 type of object (map behind statics behind dynamics behind mobiles if on same coordinates)
    renderPriority_[2] = 10;

    // level 3 tiledata value height
    renderPriority_[3] = tileDataInfo_->height_;

    // level 4 if hue is set => higher value
    renderPriority_[4] = (hue_ != 0) ? 1 : 0;

    // level 5 index in statics file
    renderPriority_[5] = indexInBlock_;
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
    LOG_INFO << "Clicked static, id=" << std::hex << getArtId() << std::dec << " loc=(" << getLocX() << "/" << getLocY() << "/" <<
            getLocZ() << ") name=" << tileDataInfo_->name_ << std::endl;
}


std::list<boost::shared_ptr<StaticItem> >& StaticBlock::getItemList() {
    return itemList_;
}


}
}
