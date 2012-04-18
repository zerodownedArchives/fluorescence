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



#include "map.hpp"

#include <client.hpp>
#include <misc/config.hpp>

#include <data/manager.hpp>
#include <data/artloader.hpp>
#include <data/maptexloader.hpp>
#include <data/tiledataloader.hpp>

#include <ui/texture.hpp>
#include <ui/manager.hpp>

namespace fluo {
namespace world {

MapTile::MapTile() : IngameObject(IngameObject::TYPE_MAP), artId_(0), isFlat_(false) {
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

    setIgnored(isIdIgnored(artId_));
    if (isIdWater(artId_)) {
        setRenderEffect(RenderEffect::WATER);
    }

    calculateIsFlat();

    setLocation(locX, locY, locZ);
}

void MapTile::updateVertexCoordinates() {
    // top left corner of a rectangle spanning the whole texture
    int px = (getLocXDraw() - getLocYDraw()) * 22;
    int py = (getLocXDraw() + getLocYDraw()) * 22;

    if (isFlat_) {
        // flat tile or tile without texture
        py -= getLocZDraw() * 4;
        CL_Rectf rect(px, py, px + 44, py + 44);

        worldRenderData_.setVertexCoordinates(rect);

        //LOG_DEBUG(LOGTYPE_UI, "Pixel coords:");
        //for (unsigned int i = 0; i < 6; ++i) {
            //LOGARG_DEBUG(LOGTYPE_UI, "%u [ %.0f %.0f ]", i, vertexCoordinates_[i].x, vertexCoordinates_[i].y);
        //}
    } else {
        // stretched texture
        worldRenderData_.setVertexCoordinates(0, px + 22,   py - getLocZDraw() * 4);
        worldRenderData_.setVertexCoordinates(1, px,        py + 22 - zLeft_ * 4);
        worldRenderData_.setVertexCoordinates(2, px + 44,   py + 22 - zRight_ * 4);
        worldRenderData_.setVertexCoordinates(3, px,        py + 22 - zLeft_ * 4);
        worldRenderData_.setVertexCoordinates(4, px + 44,   py + 22 - zRight_ * 4);
        worldRenderData_.setVertexCoordinates(5, px + 22,   py + 44 - zBottom_ * 4);
    }
}

void MapTile::calculateIsFlat() {
    isFlat_ = (zLeft_ == zRight_ && zLeft_ == zBottom_ && zLeft_ == getLocZGame()) || tileDataInfo_->textureId_ <= 0;
}

void MapTile::updateRenderDepth() {
    worldRenderData_.setRenderDepth(getLocXGame(), getLocYGame(), getLocZGame(), 0, 0, 0);
}

void MapTile::updateTextureProvider() {
    calculateIsFlat();

    if (isFlat_) {
        texture_ = data::Manager::getArtLoader()->getMapTexture(artId_);
    } else {
        texture_ = data::Manager::getMapTexLoader()->get(tileDataInfo_->textureId_);
    }
}

bool MapTile::updateAnimation(unsigned int elapsedMillis) {
    // water tile animation always changes, others are not animated
    return getRenderEffect() == RenderEffect::WATER;
}

void MapTile::setSurroundingZ(int left, int right, int bottom) {
    bool flatBefore = isFlat_;
    zLeft_ = left;
    zRight_ = right;
    zBottom_ = bottom;

    calculateIsFlat();

    if (isFlat_ != flatBefore) {
        invalidateTextureProvider();
    }
    invalidateVertexCoordinates();
}

unsigned int MapTile::getArtId() {
    return artId_;
}

const data::LandTileInfo* MapTile::getTileDataInfo() {
    return tileDataInfo_;
}

bool MapTile::overlaps(const CL_Rectf& rect) const {
    //LOGARG_DEBUG(LOGTYPE_WORLD, "isInDrawArea (%u %u %u %u) => x=%u y=%u\n", leftPixelCoord, rightPixelCoord, topPixelCoord, bottomPixelCoord, vertexCoordinates_[0u].x, vertexCoordinates_[0u].y);

    if (isFlat_) {
        return IngameObject::overlaps(rect);
    } else {
        const CL_Vec3f* vc = worldRenderData_.getVertexCoordinates();
        return vc[1].x <= rect.right &&
                (vc[0].y <= rect.bottom || vc[1].y <= rect.bottom || vc[2].y <= rect.bottom || vc[5].y <= rect.bottom) &&
                vc[4].x >= rect.left &&
                (vc[0].y >= rect.top || vc[1].y >= rect.top || vc[2].y >= rect.top || vc[5].y >= rect.top);
    }
}

bool MapTile::hasPixel(int pixelX, int pixelY) const {
    if (isFlat_) {
        return IngameObject::hasPixel(pixelX, pixelY);
    } else {
        const CL_Vec3f* vc = worldRenderData_.getVertexCoordinates();
        return isPixelInside(pixelX, pixelY, vc[0], vc[2]) &&
                isPixelInside(pixelX, pixelY, vc[2], vc[5]) &&
                isPixelInside(pixelX, pixelY, vc[5], vc[3]) &&
                isPixelInside(pixelX, pixelY, vc[3], vc[0]);
    }

}

bool MapTile::isPixelInside(int pixelX, int pixelY, const CL_Vec2f& b, const CL_Vec2f& c) const {
    return (b.x * c.y + c.x * pixelY + pixelX * b.y - c.x*b.y - pixelX * c.y - b.x * pixelY) >= 0;
}

void MapTile::setVertexNormals(const CL_Vec3f& top, const CL_Vec3f& right, const CL_Vec3f& bottom, const CL_Vec3f& left) {
    if (isFlat_) {
        worldRenderData_.vertexNormals_[0] = CL_Vec3f(0, 0, 1);
        worldRenderData_.vertexNormals_[1] = CL_Vec3f(0, 0, 1);
        worldRenderData_.vertexNormals_[2] = CL_Vec3f(0, 0, 1);
        worldRenderData_.vertexNormals_[3] = CL_Vec3f(0, 0, 1);
        worldRenderData_.vertexNormals_[4] = CL_Vec3f(0, 0, 1);
        worldRenderData_.vertexNormals_[5] = CL_Vec3f(0, 0, 1);
    } else {
        worldRenderData_.vertexNormals_[0] = top;
        worldRenderData_.vertexNormals_[1] = left;
        worldRenderData_.vertexNormals_[2] = right;
        worldRenderData_.vertexNormals_[3] = left;
        worldRenderData_.vertexNormals_[4] = right;
        worldRenderData_.vertexNormals_[5] = bottom;
    }
}

void MapTile::onClick() {
    LOG_INFO << "Clicked map, id=" << std::hex << getArtId() << std::dec << " loc=(" << getLocXGame() << "/" << getLocYGame() << "/" <<
            getLocZGame() << ") name=" << tileDataInfo_->name_ << " flat=" << isFlat_ << std::endl;

    LOG_INFO << "z value: self=" << getLocZGame() << " right=" << zRight_ << " bottom=" << zBottom_ << " left=" << zLeft_ << std::endl;
    
    LOG_INFO << "Tex coords: " << getIngameTexture()->getTextureCoords() << std::endl;
    LOG_INFO << "Normalized:" << getIngameTexture()->getNormalizedTextureCoords() << std::endl;

    const CL_Vec3f* vc = worldRenderData_.getVertexCoordinates();
    LOG_INFO << vc[0].x << " " << (unsigned int)vc[0].y << " " << vc[0].z << std::endl;
    LOG_INFO << vc[1].x << " " << (unsigned int)vc[1].y << " " << vc[1].z << std::endl;
    LOG_INFO << vc[2].x << " " << (unsigned int)vc[2].y << " " << vc[2].z << std::endl;
    LOG_INFO << vc[5].x << " " << (unsigned int)vc[5].y << " " << vc[5].z << std::endl;

    printRenderDepth();
}

float MapTile::getAverageZ() const {
    if (isFlat_) {
        return getLocZGame();
    } else {
        return (getLocZGame() + zLeft_ + zRight_ + zBottom_) / 4;
    }
}

int MapTile::getMaxZ() const {
    return (std::max)(getLocZGame(), (std::max)(zLeft_, (std::max)(zRight_, zBottom_)));
}

bool MapTile::isIdIgnored(unsigned int artId) {
    static bool initialized = false;
    static std::vector<int> ignoredIds;
    if (!initialized) {
        Config& cfg = Client::getSingleton()->getConfig();
        cfg["/fluo/specialids/ignore@mapart"].toIntList(ignoredIds);
        
        std::sort(ignoredIds.begin(), ignoredIds.end());
        
        initialized = true;
    }
    
    return std::binary_search(ignoredIds.begin(), ignoredIds.end(), artId);
}

bool MapTile::isIdWater(unsigned int artId) {
    static bool initialized = false;
    static std::vector<int> ignoredIds;
    if (!initialized) {
        Config& cfg = Client::getSingleton()->getConfig();
        cfg["/fluo/specialids/water@mapart"].toIntList(ignoredIds);
        
        std::sort(ignoredIds.begin(), ignoredIds.end());
        
        initialized = true;
    }
    
    return std::binary_search(ignoredIds.begin(), ignoredIds.end(), artId);
}


MapBlock::MapBlock() : repaintRequested_(false) {
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
