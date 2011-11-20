
#include "worldrenderdata.hpp"

#include <misc/log.hpp>

namespace fluo {
namespace ui {

WorldRenderData::WorldRenderData() :
            textureProviderUpdateRequired_(true), vertexCoordinatesUpdateRequired_(true), renderPriorityUpdateRequired_(true),
            textureProviderUpdated_(false), vertexCoordinatesUpdated_(false), renderPriorityUpdated_(false), depth_(0) {
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

void WorldRenderData::invalidateRenderPriority() {
    renderPriorityUpdateRequired_ = true;
}

bool WorldRenderData::renderDataValid() const {
    return !textureProviderUpdateRequired_ && !vertexCoordinatesUpdateRequired_ && !renderPriorityUpdateRequired_;
}

bool WorldRenderData::textureProviderUpdateRequired() const {
    return textureProviderUpdateRequired_;
}

bool WorldRenderData::vertexCoordinatesUpdateRequired() const {
    return vertexCoordinatesUpdateRequired_;
}

bool WorldRenderData::renderPriorityUpdateRequired() const {
    return renderPriorityUpdateRequired_;
}

void WorldRenderData::onTextureProviderUpdate() {
    textureProviderUpdateRequired_ = false;
    textureProviderUpdated_ = true;
}

void WorldRenderData::onVertexCoordinatesUpdate() {
    vertexCoordinatesUpdateRequired_ = false;
    vertexCoordinatesUpdated_ = true;
}

void WorldRenderData::onRenderPriorityUpdate() {
    renderPriorityUpdateRequired_ = false;
    renderPriorityUpdated_ = true;
}

bool WorldRenderData::textureProviderUpdated() const {
    return textureProviderUpdated_;
}

bool WorldRenderData::vertexCoordinatesUpdated() const {
    return vertexCoordinatesUpdated_;
}

bool WorldRenderData::renderPriorityUpdated() const {
    return renderPriorityUpdated_;
}

void WorldRenderData::reset() {
    textureProviderUpdated_ = false;
    vertexCoordinatesUpdated_ = false;
    renderPriorityUpdated_ = false;
}

void WorldRenderData::setVertexCoordinates(unsigned int idx, float x, float y) {
    vertexCoordinates_[idx].x = x;
    vertexCoordinates_[idx].y = y;
}

void WorldRenderData::setDepth(unsigned long long value) {
    depth_ = value;
}

void WorldRenderData::setDepth(uint16_t xPlusY, int8_t z, uint8_t priority, uint8_t byte5, uint8_t byte6) {
    unsigned long long tmp = xPlusY;
    tmp <<= 16;
    tmp |= z;
    tmp <<= 8;
    tmp |= priority;
    tmp <<= 8;
    tmp |= byte5;
    tmp <<= 8;
    tmp |= byte6;

    tmp &= 0xFFFFFFFFFFFF;

    //float depth = 1.0f / tmp;
    //depth *= 10000000;
    //depth *= 10000000;

    depth_ = tmp;
}

unsigned long long WorldRenderData::getDepth() const {
    return depth_;
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
