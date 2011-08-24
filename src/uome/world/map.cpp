
#include "map.hpp"

#include <data/manager.hpp>
#include <data/artloader.hpp>
#include <data/maptexloader.hpp>
#include <data/tiledataloader.hpp>

#include <ui/texture.hpp>

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

    // set surrounding z to safety values until they arrive
    zLeft_ = locZ;
    zRight_ = locZ;
    zBottom_ = locZ;

    // texture is not set here, but in updateTexture

    setLocation(locX, locY, locZ);
}

void MapTile::updateVertexCoordinates() {
    // top left corner of a rectangle spanning the whole texture
    int px = (getLocX() - getLocY()) * 22;
    int py = (getLocX() + getLocY()) * 22;

    if (isFlat()) {
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

bool MapTile::isFlat() const {
    return (zLeft_ == zRight_ && zLeft_ == zBottom_ && zLeft_ == getLocZ()) || tileDataInfo_->textureId_ <= 0;
}

void MapTile::updateRenderPriority() {
    renderPriority_[0] = getLocX() + getLocY();

    renderPriority_[1] = getLocZ();
}

void MapTile::updateTextureProvider() {
    if (isFlat()) {
        texture_ = data::Manager::getArtLoader()->getMapTexture(artId_);
    } else {
        texture_ = data::Manager::getMapTexLoader()->get(tileDataInfo_->textureId_);
    }
}

bool MapTile::updateAnimation(unsigned int elapsedMillis) {
    // no animation on maptile
    return false;
}

void MapTile::setSurroundingZ(int left, int right, int bottom) {
    zLeft_ = left;
    zRight_ = right;
    zBottom_ = bottom;

    invalidateRenderData(true);

    // if the tile got surrounding z values from the loading thread for the first time, we can finally add it to the render queue
    // double adding checks are done in the function
    addToRenderQueue();
}

unsigned int MapTile::getArtId() {
    return artId_;
}

const data::LandTileInfo* MapTile::getTileDataInfo() {
    return tileDataInfo_;
}

bool MapTile::isInDrawArea(int leftPixelCoord, int rightPixelCoord, int topPixelCoord, int bottomPixelCoord) const {
    //LOGARG_DEBUG(LOGTYPE_WORLD, "isInDrawArea (%u %u %u %u) => x=%u y=%u\n", leftPixelCoord, rightPixelCoord, topPixelCoord, bottomPixelCoord, vertexCoordinates_[0u].x, vertexCoordinates_[0u].y);

    if (isFlat()) {
        return IngameObject::isInDrawArea(leftPixelCoord, rightPixelCoord, topPixelCoord, bottomPixelCoord);
    } else {
        return vertexCoordinates_[1].x <= rightPixelCoord &&
                (vertexCoordinates_[0].y <= bottomPixelCoord || vertexCoordinates_[1].y <= bottomPixelCoord ||
                        vertexCoordinates_[2].y <= bottomPixelCoord || vertexCoordinates_[5].y <= bottomPixelCoord) &&
                vertexCoordinates_[4].x >= leftPixelCoord &&
                (vertexCoordinates_[0].y >= topPixelCoord || vertexCoordinates_[1].y >= topPixelCoord ||
                        vertexCoordinates_[2].y >= topPixelCoord || vertexCoordinates_[5].y >= topPixelCoord);
    }
}

bool MapTile::hasPixel(int pixelX, int pixelY) const {
    if (isFlat()) {
        return IngameObject::hasPixel(pixelX, pixelY);
    } else {
        return isPixelInside(pixelX, pixelY, vertexCoordinates_[0], vertexCoordinates_[2]) &&
                isPixelInside(pixelX, pixelY, vertexCoordinates_[2], vertexCoordinates_[5]) &&
                isPixelInside(pixelX, pixelY, vertexCoordinates_[5], vertexCoordinates_[3]) &&
                isPixelInside(pixelX, pixelY, vertexCoordinates_[3], vertexCoordinates_[0]);
    }

}

bool MapTile::isPixelInside(int pixelX, int pixelY, const CL_Vec2f& b, const CL_Vec2f& c) const {
    return (b.x * c.y + c.x * pixelY + pixelX * b.y - c.x*b.y - pixelX * c.y - b.x * pixelY) >= 0;
}

void MapTile::setVertexNormals(const CL_Vec3f& top, const CL_Vec3f& right, const CL_Vec3f& bottom, const CL_Vec3f& left) {
    if (isFlat()) {
        vertexNormals_[0] = CL_Vec3f(0, 0, 1);
        vertexNormals_[1] = CL_Vec3f(0, 0, 1);
        vertexNormals_[2] = CL_Vec3f(0, 0, 1);
        vertexNormals_[3] = CL_Vec3f(0, 0, 1);
        vertexNormals_[4] = CL_Vec3f(0, 0, 1);
        vertexNormals_[5] = CL_Vec3f(0, 0, 1);
    } else {
        vertexNormals_[0] = top;
        vertexNormals_[1] = left;
        vertexNormals_[2] = right;
        vertexNormals_[3] = left;
        vertexNormals_[4] = right;
        vertexNormals_[5] = bottom;
    }
}

void MapTile::onClick() {
    LOG_INFO << "Clicked map, id=" << std::hex << getArtId() << std::dec << " loc=(" << getLocX() << "/" << getLocY() << "/" <<
            getLocZ() << ") name=" << tileDataInfo_->name_ << std::endl;

    //LOG_INFO << "z value: self=" << getLocZ() << " right=" << zRight_ << " bottom=" << zBottom_ << " left=" << zLeft_ << std::endl;
}


MapBlock::MapBlock() {
    for (unsigned int i = 0; i < 64; ++i) {
        tiles_[i].reset(new MapTile());
    }
}

boost::shared_ptr<MapTile> MapBlock::get(unsigned int x, unsigned int y) {
    if (x > 7) {
        x = 0;
    }

    if (y > 7) {
        y = 0;
    }

    return tiles_[(y*8) + x];
}

}
}
