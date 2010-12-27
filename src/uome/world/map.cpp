
#include "map.hpp"

#include <data/manager.hpp>
#include <data/artloader.hpp>
#include <data/maptexloader.hpp>
#include <data/tiledataloader.hpp>

namespace uome {
namespace world {

MapTile::MapTile() : artId_(0) {
}

boost::shared_ptr<ui::Texture> MapTile::getIngameTexture() const {
    return texture_;
}

void MapTile::set(int locX, int locY, int locZ, unsigned int artId) {
    artId_ = artId;
    tileDataInfo_ = data::Manager::getTileDataLoader()->getLandTileInfo(artId_);

    // texture is not set here, but in updateTexture

    setLocation(locX, locY, locZ);
}

void MapTile::updateVertexCoordinates() {
    // top left corner of a rectangle spanning the whole texture
    int px = (getLocX() - getLocY()) * 22;
    int py = (getLocX() + getLocY()) * 22;

    if ((zLeft_ == zRight_ && zLeft_ == zBottom_ && zLeft_ == getLocZ()) || tileDataInfo_->textureId_ < 0) {
        // flat tile or tile without texture
        py -= getLocZ() * 4;
        CL_Rectf rect(px, py, px + 44, py + 44);

        vertexCoordinates_[0] = CL_Vec2f(rect.left, rect.top);
        vertexCoordinates_[1] = CL_Vec2f(rect.right, rect.top);
        vertexCoordinates_[2] = CL_Vec2f(rect.left, rect.bottom);
        vertexCoordinates_[3] = CL_Vec2f(rect.right, rect.top);
        vertexCoordinates_[4] = CL_Vec2f(rect.left, rect.bottom);
        vertexCoordinates_[5] = CL_Vec2f(rect.right, rect.bottom);

        //LOG_DEBUG(LOGTYPE_UI, "Pixel coords:");
        //for (unsigned int i = 0; i < 6; ++i) {
            //LOGARG_DEBUG(LOGTYPE_UI, "%u [ %.0f %.0f ]", i, vertexCoordinates_[i].x, vertexCoordinates_[i].y);
        //}
    } else {
        // stretched texture
        vertexCoordinates_[0] = CL_Vec2f(px + 22,   py - getLocZ() * 4);
        vertexCoordinates_[1] = CL_Vec2f(px,        py + 22 - zLeft_ * 4);
        vertexCoordinates_[2] = CL_Vec2f(px + 44,   py + 22 - zRight_ * 4);
        vertexCoordinates_[3] = CL_Vec2f(px,        py + 22 - zLeft_ * 4);
        vertexCoordinates_[4] = CL_Vec2f(px + 44,   py + 22 - zRight_ * 4);
        vertexCoordinates_[5] = CL_Vec2f(px + 22,   py + 44 - zBottom_ * 4);
    }
}

void MapTile::updateRenderPriority() {
    renderPriority_[0] = getLocX() + getLocY();

    renderPriority_[1] = getLocZ();
}

void MapTile::updateTextureProvider() {
    if ((zLeft_ == zRight_ && zLeft_ == zBottom_ && zLeft_ == getLocZ()) || tileDataInfo_->textureId_ < 0) {
        texture_ = data::Manager::getArtLoader()->getMapTexture(artId_);
    } else {
        texture_ = data::Manager::getMapTexLoader()->get(tileDataInfo_->textureId_);
    }
}

void MapTile::setRightZ(int right) {
    setSurroundingZ(zLeft_, right, zBottom_);
}

void MapTile::setSurroundingZ(int left, int right, int bottom) {
    zLeft_ = left;
    zRight_ = right;
    zBottom_ = bottom;

    invalidateRenderData();

    // if the tile got a texture for the first time, we can finally add it to the render queue
    if (texture_.get() == NULL) {
        addToRenderQueue();
    }
}

MapTile* MapBlock::get(unsigned int x, unsigned int y) {
    if (x > 7) {
        x = 0;
    }

    if (y > 7) {
        y = 0;
    }

    return &tiles_[(y*8) + x];
}

}
}
