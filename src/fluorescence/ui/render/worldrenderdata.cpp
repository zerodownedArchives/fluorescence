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



#include "worldrenderdata.hpp"

#include <float.h>

#include <misc/log.hpp>

namespace fluo {
namespace ui {

WorldRenderData::WorldRenderData() :
            textureProviderUpdateRequired_(true), vertexCoordinatesUpdateRequired_(true), renderDepthUpdateRequired_(true), vertexRectFast_(false) {
    for (unsigned int i = 0; i < 6; ++i) {
        vertexNormals_[i] = CL_Vec3f(0, 0, 1);
    }
    hueInfo_ = CL_Vec3f(0, 0, 1);
}

void WorldRenderData::invalidateTextureProvider() {
    textureProviderUpdateRequired_ = true;
}

void WorldRenderData::invalidateVertexCoordinates() {
    vertexCoordinatesUpdateRequired_ = true;
}

void WorldRenderData::invalidateRenderDepth() {
    renderDepthUpdateRequired_ = true;
}

bool WorldRenderData::renderDataValid() const {
    return !textureProviderUpdateRequired_ && !vertexCoordinatesUpdateRequired_ && !renderDepthUpdateRequired_;
}

bool WorldRenderData::textureProviderUpdateRequired() const {
    return textureProviderUpdateRequired_;
}

bool WorldRenderData::vertexCoordinatesUpdateRequired() const {
    return vertexCoordinatesUpdateRequired_;
}

bool WorldRenderData::renderDepthUpdateRequired() const {
    return renderDepthUpdateRequired_;
}

void WorldRenderData::onTextureProviderUpdate() {
    textureProviderUpdateRequired_ = false;
    textureOrVerticesUpdated_ = true;
}

void WorldRenderData::onVertexCoordinatesUpdate() {
    vertexCoordinatesUpdateRequired_ = false;
    textureOrVerticesUpdated_ = true;
}

void WorldRenderData::onRenderDepthUpdate() {
    renderDepthUpdateRequired_ = false;
    renderDepthUpdated_ = true;
}

void WorldRenderData::setVertexCoordinates(unsigned int idx, float x, float y) {
    vertexCoordinates_[idx].x = x;
    vertexCoordinates_[idx].y = y;
    
    vertexRectFast_ = false;
}

void WorldRenderData::setRenderDepth(uint16_t x, uint16_t y, int8_t z, uint8_t priority, uint8_t byte7, uint8_t byte8) { 
    uint64_t tmp = 0;
    tmp |= ((x+y) & 0xFFFF);
    tmp <<= 8;
    uint8_t tmpZ = (((int)z) + 128) & 0xFF;
    tmp |= (tmpZ);
    tmp <<= 8;
    tmp |= (priority & 0xFF);
    tmp <<= 8;
    tmp |= (byte7 & 0xFF);
    tmp <<= 8;
    tmp |= (byte8 & 0xFF);

    renderDepth_ = tmp;
}

const RenderDepth& WorldRenderData::getRenderDepth() const {
    return renderDepth_;
}

const CL_Vec3f* WorldRenderData::getVertexCoordinates() const {
    return vertexCoordinates_;
}

void WorldRenderData::setVertexCoordinates(const CL_Rectf& rect) {
    vertexCoordinates_[0].x = rect.left;
    vertexCoordinates_[0].y = rect.top;
    vertexCoordinates_[1].x = rect.right;
    vertexCoordinates_[1].y = rect.top;
    vertexCoordinates_[2].x = rect.left;
    vertexCoordinates_[2].y = rect.bottom;
    vertexCoordinates_[3].x = rect.right;
    vertexCoordinates_[3].y = rect.top;
    vertexCoordinates_[4].x = rect.left;
    vertexCoordinates_[4].y = rect.bottom;
    vertexCoordinates_[5].x = rect.right;
    vertexCoordinates_[5].y = rect.bottom;
    
    vertexRectFast_ = true;
}

void WorldRenderData::resetPreUpdate() {
    renderDepthUpdated_ = false;
    textureOrVerticesUpdated_ = false;
    
    if (!renderDataValid()) {
        // store last vertex coordinates for clipped updates
        previousVertexRect_ = getCurrentVertexRect();
    }
}

CL_Rectf WorldRenderData::getCurrentVertexRectFast() const {
    return CL_Rectf(vertexCoordinates_[0].x, vertexCoordinates_[0].y, vertexCoordinates_[5].x, vertexCoordinates_[5].y);
};

CL_Rectf WorldRenderData::getCurrentVertexRect() const {
    if (vertexRectFast_) {
        return getCurrentVertexRectFast();
    }
    
    float xmin = FLT_MAX;
    float xmax = FLT_MIN;
    float ymin = FLT_MAX;
    float ymax = FLT_MIN;
    
    for (unsigned int i = 0; i < 6; ++i) {
        xmin = (std::min)(xmin, vertexCoordinates_[i].x);
        xmax = (std::max)(xmax, vertexCoordinates_[i].x);
        ymin = (std::min)(ymin, vertexCoordinates_[i].y);
        ymax = (std::max)(ymax, vertexCoordinates_[i].y);
    }
    
    return CL_Rectf(xmin, ymin, xmax, ymax);
}

bool WorldRenderData::textureOrVerticesUpdated() const {
    return textureOrVerticesUpdated_;
}

bool WorldRenderData::renderDepthUpdated() const {
    return renderDepthUpdated_;
}

}
}
