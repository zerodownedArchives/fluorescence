
#include "ingameobject.hpp"

#include <ui/manager.hpp>

namespace uome {
namespace world {

IngameObject::IngameObject() : visible_(false), renderDataValid_(false) {
    for (unsigned int i = 0; i < 6; ++i) {
        renderPriority_[i] = 0;
    }
}

IngameObject::~IngameObject() {
    // remove from render queue is first action in dtor
    ui::Manager::getSingleton()->getRenderQueue()->remove(this);
}

void IngameObject::setLocation(int locX, int locY, int locZ) {
    CL_Vec3<int> oldLocation = location_;

    location_ = CL_Vec3<int>(locX, locY, locZ);

    invalidateRenderData();
}

bool IngameObject::isRenderDataValid() const {
    return renderDataValid_;
}

void IngameObject::invalidateRenderData() {
    renderDataValid_ = false;
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

void IngameObject::updateRenderData() {
    updateVertexCoordinates();
    updateRenderPriority();

    renderDataValid_ = true;

    // the rendering order might have been changed
    ui::Manager::getSingleton()->getRenderQueue()->requireSort();
}

void IngameObject::addToRenderQueue() {
    ui::Manager::getSingleton()->getRenderQueue()->add(this);
}

}
}
