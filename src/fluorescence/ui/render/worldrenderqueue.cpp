
#include "worldrenderqueue.hpp"

#include <boost/bind.hpp>

#include <misc/log.hpp>

#include <world/ingameobject.hpp>
#include <world/sector.hpp>

#include <world/manager.hpp>
#include <world/dynamicitem.hpp>

namespace fluo {
namespace ui {

bool WorldRenderQueue::renderPriorityComparator(const boost::shared_ptr<world::IngameObject>& a, const boost::shared_ptr<world::IngameObject>& b) {
    const int* aPrio = a->getRenderPriorities();
    const int* bPrio = b->getRenderPriorities();

    if (aPrio[0] > bPrio[0]) {
        return false;
    }

    for (int i=1; i < 6; ++i) {
        if (aPrio[i-1] == bPrio[i-1]) {
            if (aPrio[i] > bPrio[i]) {
                return false;
            }
        } else {
            return true;
        }
    }

    if (aPrio[5] < bPrio[5]) {
        return true;
    }

    /* None of the priorities differs. To make sure this function sorts the list exactly the same, no matter how the
     * list elements were in the list before, we use the memory address of the objects as a last resort
     */
    return (unsigned long)a.get() <= (unsigned long)b.get();
}

WorldRenderQueue::WorldRenderQueue() : RenderQueue(boost::bind(&WorldRenderQueue::renderPriorityComparator, this, _1, _2)) {
}

WorldRenderQueue::~WorldRenderQueue() {
}


void WorldRenderQueue::preRender() {
    if (!removeList_.empty()) {
        processRemoveList();
        forceRepaint_ = true;
    }

    bool requireSort = processAddList();

    if (requireSort || worldPriorityChanged_ ) {
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
