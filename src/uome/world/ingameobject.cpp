
#include "ingameobject.hpp"

#include <stdio.h>

#include <ui/texture.hpp>
#include <ui/manager.hpp>
#include <ui/renderqueue.hpp>

namespace uome {
namespace world {

IngameObject::IngameObject() : visible_(true), renderDataValid_(false), textureProviderUpdateRequired_(true), addedToRenderQueue_(false) {
    for (unsigned int i = 0; i < 6; ++i) {
        renderPriority_[i] = 0;
    }
}

IngameObject::~IngameObject() {
    // remove from render queue is first action in dtor
    removeFromRenderQueue();
}

void IngameObject::setLocation(int locX, int locY, int locZ) {
    CL_Vec3<int> oldLocation = location_;

    location_ = CL_Vec3<int>(locX, locY, locZ);

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

void IngameObject::updateRenderData() {
    if (textureProviderUpdateRequired_) {
        updateTextureProvider();
        textureProviderUpdateRequired_ = false;
    }

    boost::shared_ptr<ui::Texture> tex = getIngameTexture();
    if (!tex->isReadComplete()) {
        return;
    }

    updateVertexCoordinates();
    updateRenderPriority();

    renderDataValid_ = true;

    // the rendering order might have been changed
    ui::Manager::getSingleton()->getRenderQueue()->requireSort();
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

void IngameObject::removeFromRenderQueue() {
    if (addedToRenderQueue_) {
        ui::Manager::getSingleton()->getRenderQueue()->remove(this);
        addedToRenderQueue_ = false;
    }
}

bool IngameObject::isInDrawArea(int leftPixelCoord, int rightPixelCoord, int topPixelCoord, int bottomPixelCoord) const {
    // this code is not pretty. but as this function is called _very_ often, it is optimized a little
    bool leftOk = false;
    bool rightOk = false;
    bool topOk = false;
    bool bottomOk = false;

    for (unsigned int i = 0; i < 6; ++i) {
        if (vertexCoordinates_[i].x >= leftPixelCoord) {
            leftOk = true;
            break;
        }
    }

    if (!leftOk) {
        return false;
    }

    for (unsigned int i = 0; i < 6; ++i) {
        if (vertexCoordinates_[i].x <= rightPixelCoord) {
            rightOk = true;
            break;
        }
    }

    if (!rightOk) {
        return false;
    }

    for (unsigned int i = 0; i < 6; ++i) {
        if (vertexCoordinates_[i].y >= topPixelCoord) {
            topOk = true;
            break;
        }
    }

    if (!topOk) {
        return false;
    }

    for (unsigned int i = 0; i < 6; ++i) {
        if (vertexCoordinates_[i].y <= bottomPixelCoord) {
            bottomOk = true;
            break;
        }
    }

    return bottomOk;

    // unoptimized code:

    //for (unsigned int i = 0; i <6; ++i) {
        //leftOk = leftOk || vertexCoordinates_[i].x >= leftPixelCoord;
        //rightOk = rightOk || vertexCoordinates_[i].x <= rightPixelCoord;

        //topOk = topOk || vertexCoordinates_[i].y >= topPixelCoord;
        //bottomOk = bottomOk || vertexCoordinates_[i].y <= bottomPixelCoord;
    //}

    //return leftOk && rightOk && topOk && bottomOk;
}

}
}
