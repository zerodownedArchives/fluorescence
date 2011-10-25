
#include "worldrenderdata.hpp"

namespace fluo {
namespace ui {

WorldRenderData::WorldRenderData() :
            textureProviderUpdateRequired_(true), vertexCoordinatesUpdateRequired_(true), renderPriorityUpdateRequired_(true),
            textureProviderUpdated_(false), vertexCoordinatesUpdated_(false), renderPriorityUpdated_(false) {
    for (unsigned int i = 0; i < 6; ++i) {
        renderPriority_[i] = 0;
        vertexNormals_[i] = CL_Vec3f(0, 0, 1);
    }
    hueInfo_ = CL_Vec3f(0.0f, 0.0f, 1.0f);
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

}
}
