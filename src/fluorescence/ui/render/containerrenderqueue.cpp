
#include "containerrenderqueue.hpp"

#include <boost/bind.hpp>

#include <client.hpp>
#include <misc/log.hpp>

#include <world/ingameobject.hpp>
#include <world/sector.hpp>

#include <world/manager.hpp>
#include <world/dynamicitem.hpp>

namespace fluo {
namespace ui {

bool ContainerRenderQueue::renderDepthComparator(const boost::shared_ptr<world::IngameObject>& a, const boost::shared_ptr<world::IngameObject>& b) {
    if (!a->isDynamicItem() || !b->isDynamicItem()) {
        // why is that in this queue?
        LOG_WARN << "Object other than item in ContainerRenderQueue" << std::endl;
        return true;
    }

    boost::shared_ptr<world::DynamicItem> aDyn = boost::dynamic_pointer_cast<world::DynamicItem>(a);
    boost::shared_ptr<world::DynamicItem> bDyn = boost::dynamic_pointer_cast<world::DynamicItem>(b);

    return aDyn->getSerial() <= bDyn->getSerial();
}

ContainerRenderQueue::ContainerRenderQueue() : RenderQueue(boost::bind(&ContainerRenderQueue::renderDepthComparator, this, _1, _2)) {
}

ContainerRenderQueue::~ContainerRenderQueue() {
}


void ContainerRenderQueue::preRender() {
    if (!removeList_.empty()) {
        processRemoveList();
        forceRepaint_ = true;
    }

    bool requireSort = processAddList();

    if (requireSort) {
        sort();
    }
}

void ContainerRenderQueue::postRender(bool renderingComplete) {
    resetWorldRepaintIndicators();

    // force repaint if the rendering was not complete (e.g. a texture not yet loaded)
    forceRepaint_ = !renderingComplete;
}

boost::shared_ptr<world::IngameObject> ContainerRenderQueue::getFirstObjectAt(int pixelX, int pixelY, bool getTopParent) {
    RenderQueue::reverse_iterator igIter = rbegin();
    RenderQueue::reverse_iterator igEnd = rend();

    boost::shared_ptr<world::IngameObject> ret;

    for (; igIter != igEnd; ++igIter) {
        boost::shared_ptr<world::IngameObject> curObj = *igIter;
        if (curObj->isDynamicItem() && curObj->isVisible() && curObj->hasPixel(pixelX, pixelY)) {

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
