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

#include <misc/log.hpp>

namespace fluo {
namespace ui {

WorldRenderData::WorldRenderData() :
            textureProviderUpdateRequired_(true), vertexCoordinatesUpdateRequired_(true), renderDepthUpdateRequired_(true) {
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
}

void WorldRenderData::onVertexCoordinatesUpdate() {
    vertexCoordinatesUpdateRequired_ = false;
}

void WorldRenderData::onRenderDepthUpdate() {
    renderDepthUpdateRequired_ = false;
}

void WorldRenderData::setVertexCoordinates(unsigned int idx, float x, float y) {
    vertexCoordinates_[idx].x = x;
    vertexCoordinates_[idx].y = y;
}

void WorldRenderData::setRenderDepth(const RenderDepth& value) {
    renderDepth_ = value;
}

void WorldRenderData::setRenderDepth(uint16_t xPlusY, int8_t z, uint8_t priority, uint8_t byte5, uint8_t byte6) { 
    uint64_t tmp = 0;
    tmp |= (xPlusY & 0xFFFF);
    tmp <<= 8;
    uint8_t tmpZ = (((int)z) + 128) & 0xFF;
    tmp |= (tmpZ);
    tmp <<= 8;
    tmp |= (priority & 0xFF);
    tmp <<= 8;
    tmp |= (byte5 & 0xFF);
    tmp <<= 8;
    tmp |= (byte6 & 0xFF);

    renderDepth_ = RenderDepth(tmp);
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
}

}
}
