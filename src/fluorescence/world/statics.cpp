
#include "statics.hpp"

#include <data/manager.hpp>
#include <data/artloader.hpp>
#include <data/tiledataloader.hpp>
#include <data/huesloader.hpp>

#include <ui/texture.hpp>
#include <ui/manager.hpp>

namespace fluo {
namespace world {

StaticItem::StaticItem() : IngameObject(IngameObject::TYPE_STATIC_ITEM) {
}

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

    worldRenderData_.hueInfo_[0u] = tileDataInfo_->partialHue() ? 1.0 : 0.0;
    worldRenderData_.hueInfo_[1u] = data::Manager::getHuesLoader()->translateHue(hue_);
    worldRenderData_.hueInfo_[2u] = tileDataInfo_->translucent() ? 0.8 : 1.0;
}

void StaticItem::updateVertexCoordinates() {
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

void StaticItem::updateRenderPriority() {
    // render prio
    // level 0 x+y
    worldRenderData_.renderPriority_[0] = getLocX() + getLocY();

    // level 1 z and tiledata flags
    worldRenderData_.renderPriority_[1] = getLocZ();
    if (tileDataInfo_->background() && tileDataInfo_->surface()) {
        worldRenderData_.renderPriority_[1] += 4;
    } else if (tileDataInfo_->background()) {
        worldRenderData_.renderPriority_[1] += 2;
    } else if (tileDataInfo_->surface()) {
        worldRenderData_.renderPriority_[1] += 5;
    } else {
        worldRenderData_.renderPriority_[1] += 6;
    }

    // level 2 type of object (map behind statics behind dynamics behind mobiles if on same coordinates)
    worldRenderData_.renderPriority_[2] = 10;

    // level 3 tiledata value height
    worldRenderData_.renderPriority_[3] = tileDataInfo_->height_;

    // level 4 if hue is set => higher value
    worldRenderData_.renderPriority_[4] = (hue_ != 0) ? 1 : 0;

    // level 5 index in statics file
    worldRenderData_.renderPriority_[5] = indexInBlock_;
}

void StaticItem::updateTextureProvider() {
    bool hasProvider = (bool)textureProvider_;

    textureProvider_ = data::Manager::getItemTextureProvider(artId_);

    if (!hasProvider) {
        // texture assigned for the first time
        addToRenderQueue(ui::Manager::getWorldRenderQueue());
    }
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

    //LOG_INFO << "background=" << tileDataInfo_->background() << " surface=" << tileDataInfo_->surface() << " height=" << (unsigned int)tileDataInfo_->height_ <<
            //" hue=" << hue_ << " indexInBlock=" << indexInBlock_ << std::endl;

    //printRenderPriority();
}

bool StaticItem::requireRenderUpdate() const {
    return tileDataInfo_->animation();
}


std::list<boost::shared_ptr<StaticItem> >& StaticBlock::getItemList() {
    return itemList_;
}


}
}
