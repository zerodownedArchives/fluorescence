
#include "worldrenderqueue.hpp"

#include <boost/bind.hpp>

#include <misc/log.hpp>

#include <world/ingameobject.hpp>
#include <world/sector.hpp>

#include <world/manager.hpp>
#include <world/dynamicitem.hpp>

namespace fluo {
namespace ui {

bool WorldRenderQueue::batchedComparator(const boost::shared_ptr<world::IngameObject>& a, const boost::shared_ptr<world::IngameObject>& b) {
    unsigned long texA = (unsigned long)a->getIngameTexture().get();
    unsigned long texB = (unsigned long)b->getIngameTexture().get();

    if (texA == texB) {
        return (unsigned long)a.get() <= (unsigned long)b.get();
    } else {
        return texA <= texB;
    }
}

WorldRenderQueue::WorldRenderQueue() : RenderQueue(boost::bind(&WorldRenderQueue::batchedComparator, this, _1, _2)) {
}

WorldRenderQueue::~WorldRenderQueue() {
}


void WorldRenderQueue::preRender() {
    if (!removeList_.empty()) {
        sort();
        processRemoveList();
        forceRepaint_ = true;
    }

    if (!addList_.empty()) {
        updateMinMaxRenderPriority();
    }

    bool objectsAdded = processAddList();

    if (objectsAdded) {
        sort();
    }
}

void WorldRenderQueue::postRender(bool renderingComplete) {
    resetWorldRepaintIndicators();

    // force repaint if the rendering was not complete (e.g. a texture not yet loaded)
    forceRepaint_ = !renderingComplete;
}

boost::shared_ptr<world::IngameObject> WorldRenderQueue::getFirstObjectAt(int worldX, int worldY, bool getTopParent) {
    RenderQueue::reverse_iterator igIter = rbegin();
    RenderQueue::reverse_iterator igEnd = rend();

    boost::shared_ptr<world::IngameObject> ret;

    for (; igIter != igEnd; ++igIter) {
        boost::shared_ptr<world::IngameObject> curObj = *igIter;
        if (curObj->isVisible() && curObj->hasPixel(worldX, worldY)) {

            if (getTopParent) {
                curObj = curObj->getTopParent();
            }

            ret = curObj;
            break;
        }
    }

    return ret;
}

}
}
