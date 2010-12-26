
#include "renderqueue.hpp"

#include <logger.hpp>
#include <world/ingameobject.hpp>

namespace uome {
namespace ui {

bool RenderQueue::renderPriorityComparator(const world::IngameObject* a, const world::IngameObject* b) {
    if (a->getRenderPriority(0) < b->getRenderPriority(0)) {
        return true;
    }

    for (int i=1; i < 6; ++i) {
        if (a->getRenderPriority(i-1) == b->getRenderPriority(i-1)) {
            if (a->getRenderPriority(i) < b->getRenderPriority(i)) {
                return true;
            }
        } else {
            return false;
        }
    }
    return false;
}

RenderQueue::RenderQueue() : sorted_(true) {
}

RenderQueue::~RenderQueue() {
}

void RenderQueue::add(world::IngameObject* obj) {
    boost::mutex::scoped_lock myLock(ingameAddListMutex_);
    ingameAddList_.push_back(obj);
}

void RenderQueue::remove(world::IngameObject* obj) {
    ingameList_.remove(obj);
}

void RenderQueue::requireSort() {
    sorted_ = false;
}

void RenderQueue::prepareRender() {
    {
        boost::mutex::scoped_lock myLock(ingameAddListMutex_);
        if (!ingameAddList_.empty()) {
            ingameList_.insert(ingameList_.end(), ingameAddList_.begin(), ingameAddList_.end());
            ingameAddList_.clear();
            sorted_ = false;
        }
    }

    if (!sorted_) {
        ingameList_.sort(renderPriorityComparator);
        sorted_ = true;
    }
}

std::list<world::IngameObject*>::iterator RenderQueue::beginIngame() {
    return ingameList_.begin();
}

std::list<world::IngameObject*>::iterator RenderQueue::endIngame() {
    return ingameList_.end();
}

}
}
