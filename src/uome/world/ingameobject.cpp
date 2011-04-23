
#include "ingameobject.hpp"

#include <stdio.h>

#include <misc/logger.hpp>

#include <ui/texture.hpp>
#include <ui/manager.hpp>
#include <ui/renderqueue.hpp>

namespace uome {
namespace world {

IngameObject::IngameObject() : visible_(true), renderDataValid_(false), textureProviderUpdateRequired_(true), addedToRenderQueue_(false) {
    for (unsigned int i = 0; i < 6; ++i) {
        renderPriority_[i] = 0;
        vertexNormals_[i] = CL_Vec3f(0, 0, 1);
    }
}

IngameObject::~IngameObject() {
    // emergency remove from render queue is first action in dtor
    // this should never be necessary, but it doesn't hurt to make sure
    removeFromRenderQueueImmediately();
}

bool IngameObject::isVisible() const {
    return visible_;
}

void IngameObject::setVisible(bool visible) {
    visible_ = visible;
}

void IngameObject::setLocation(int locX, int locY, int locZ) {
    CL_Vec3<int> oldLocation = location_;

    location_ = CL_Vec3<int>(locX, locY, locZ);

    //LOGARG_DEBUG(LOGTYPE_WORLD, "Object location: %i %i %i", locX, locY, locZ);

    invalidateRenderData();
}

bool IngameObject::isRenderDataValid() const {
    return renderDataValid_;
}

void IngameObject::invalidateRenderData(bool updateTextureProvider) {
    renderDataValid_ = false;

    if (updateTextureProvider) {
        requestUpdateTextureProvider();
    }
}

const CL_Vec2f* IngameObject::getVertexCoordinates() const {
    return vertexCoordinates_;
}

int IngameObject::getRenderPriority(unsigned int lvl) const {
    if (lvl > 5) {
        lvl = 0;
    }

    return renderPriority_[lvl];
}

const int* IngameObject::getRenderPriorities() const {
    return renderPriority_;
}

bool IngameObject::updateRenderData(unsigned int elapsedMillis) {
    bool bigUpdate = false;
    if (!isRenderDataValid()) {
        bigUpdate = true;
        if (textureProviderUpdateRequired_) {
            updateTextureProvider();
            textureProviderUpdateRequired_ = false;
        }

        boost::shared_ptr<ui::Texture> tex = getIngameTexture();
        if (!tex->isReadComplete()) {
            return false;
        }

        updateAnimation(elapsedMillis);

        updateVertexCoordinates();
        updateRenderPriority();

        renderDataValid_ = true;
    } else {
        bool frameChanged = updateAnimation(elapsedMillis);

        if (frameChanged) {
            updateVertexCoordinates();
        }
    }

    return bigUpdate;
}

void IngameObject::requestUpdateTextureProvider() {
    textureProviderUpdateRequired_ = true;
}

void IngameObject::addToRenderQueue() {
    if (!addedToRenderQueue_) {
        ui::Manager::getSingleton()->getRenderQueue()->add(this);
        addedToRenderQueue_ = true;
    }
}

void IngameObject::removeFromRenderQueueImmediately() {
    if (addedToRenderQueue_) {
        ui::Manager::getSingleton()->getRenderQueue()->removeImmediately(this);
        addedToRenderQueue_ = false;
    }
}

bool IngameObject::isInDrawArea(int leftPixelCoord, int rightPixelCoord, int topPixelCoord, int bottomPixelCoord) const {
    //LOGARG_DEBUG(LOGTYPE_WORLD, "isInDrawArea (%u %u %u %u) => x=%u y=%u\n", leftPixelCoord, rightPixelCoord, topPixelCoord, bottomPixelCoord, vertexCoordinates_[0u].x, vertexCoordinates_[0u].y);

    // almost every texture is a rectangle, with vertexCoordinates_[0] being top left and vertexCoordinates_[5] bottom right
    // all non-rectangular cases are maptiles
    return vertexCoordinates_[0].x <= rightPixelCoord &&
            vertexCoordinates_[0].y <= bottomPixelCoord &&
            vertexCoordinates_[5].x >= leftPixelCoord &&
            vertexCoordinates_[5].y >= topPixelCoord;
}

bool IngameObject::hasPixel(int pixelX, int pixelY) const {
    // almost every texture is a rectangle, with vertexCoordinates_[0] being top left and vertexCoordinates_[5] bottom right
    // all non-rectangular cases are maptiles
    bool coordinateCheck = vertexCoordinates_[0].x <= pixelX &&
            vertexCoordinates_[0].y <= pixelY &&
            vertexCoordinates_[5].x >= pixelX &&
            vertexCoordinates_[5].y >= pixelY;

    boost::shared_ptr<ui::Texture> tex = getIngameTexture();
    if (coordinateCheck && tex && tex->isReadComplete()) {
        unsigned int texPixelX = pixelX - vertexCoordinates_[0].x;
        unsigned int texPixelY = pixelY - vertexCoordinates_[0].y;
        return tex->hasPixel(texPixelX, texPixelY);
    } else {
        return false;
    }
}

const CL_Vec3f* IngameObject::getVertexNormals() const {
    return vertexNormals_;
}

void IngameObject::onClick() {
}

void IngameObject::onDoubleClick() {
    setVisible(false);
}

}
}
