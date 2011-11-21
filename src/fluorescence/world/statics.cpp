
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

    worldRenderData_.setVertexCoordinates(rect);
}

void StaticItem::updateRenderPriority() {
    // render prio
    // level 0 x+y
    uint16_t xy = ceilf(getLocX()) + ceilf(getLocY());

    // level 1 z and tiledata flags
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

    worldRenderData_.setDepth(xy, z, 10, tileDataInfo_->height_, indexInBlock_);
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
            getLocZ() << ") name=" << " hue=" << hue_ << " " << tileDataInfo_->name_ << std::endl;

    //LOG_INFO << "background=" << tileDataInfo_->background() << " surface=" << tileDataInfo_->surface() << " height=" << (unsigned int)tileDataInfo_->height_ <<
            //" hue=" << hue_ << " indexInBlock=" << indexInBlock_ << std::endl;

    printRenderPriority();
}

bool StaticItem::requireRenderUpdate() const {
    return tileDataInfo_->animation();
}


std::list<boost::shared_ptr<StaticItem> >& StaticBlock::getItemList() {
    return itemList_;
}


}
}
