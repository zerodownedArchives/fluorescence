
#include "gumprenderqueue.hpp"

#include <boost/bind.hpp>

#include <misc/log.hpp>

#include <world/ingameobject.hpp>
#include <world/sector.hpp>

#include <world/manager.hpp>
#include <world/dynamicitem.hpp>

namespace fluo {
namespace ui {

bool GumpRenderQueue::renderPriorityComparator(const boost::shared_ptr<world::IngameObject>& a, const boost::shared_ptr<world::IngameObject>& b) {
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

    /* None of the priorities differs. To make sure this function sorts the list exactly the same, no matter how the
     * list elements were in the list before, we use the memory address of the objects as a last resort
     */
    return (unsigned long)a.get() <= (unsigned long)b.get();
}

GumpRenderQueue::GumpRenderQueue() : RenderQueue(boost::bind(&GumpRenderQueue::renderPriorityComparator, this, _1, _2)) {
}

GumpRenderQueue::~GumpRenderQueue() {
}


void GumpRenderQueue::preRender() {
    //processRemoveList();
    //bool requireSort = processAddList();

    //RenderQueue::iterator igIter = begin();
    //RenderQueue::iterator igEnd = end();

    //resetGumpRepaintIndicators();

    //for (; igIter != igEnd; ++igIter) {
        //// update rendering data (priority, vertex coordinates, texture, ...) if necessary
        //(*igIter)->updateGumpRenderData(elapsedMillis);
    //}

    //if (requireSort || objectGumpPriorityChanged_ ) {
        //sort();
        //objectWorldPriorityChanged_ = false;
    //}
}

void GumpRenderQueue::postRender() {
}

boost::shared_ptr<world::IngameObject> GumpRenderQueue::getFirstObjectAt(int worldX, int worldY, bool getTopParent) {
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
