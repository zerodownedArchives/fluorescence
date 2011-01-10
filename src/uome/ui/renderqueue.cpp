
#include "renderqueue.hpp"

#include <boost/bind.hpp>

#include <misc/logger.hpp>

#include <world/ingameobject.hpp>
#include <world/sector.hpp>

namespace uome {
namespace ui {

bool RenderQueue::renderPriorityComparatorSharedPtr(const boost::shared_ptr<world::IngameObject>& a, const boost::shared_ptr<world::IngameObject>& b) {
    return renderPriorityComparator(a.get(), b.get());
}

bool RenderQueue::renderPriorityComparator(const world::IngameObject* a, const world::IngameObject* b) {
    const int* aPrio = a->getRenderPriorities();
    const int* bPrio = b->getRenderPriorities();

    if (aPrio[0] < bPrio[0]) {
        return true;
    }

    for (int i=1; i < 6; ++i) {
        if (aPrio[i-1] == bPrio[i-1]) {
            if (aPrio[i] < bPrio[i]) {
                return true;
            }
        } else {
            return false;
        }
    }

    /* None of the priorities differs. To make sure this function sorts the list exactly the same, no matter how the
     * list elements were in the list before, we use the memory address of the objects as a last resort
     */
    return (unsigned int)a < (unsigned int)b;
}

RenderQueue::RenderQueue() {
}

RenderQueue::~RenderQueue() {
}

void RenderQueue::add(world::IngameObject* obj) {
    boost::mutex::scoped_lock myLock(ingameAddListMutex_);
    ingameAddList_.push_back(obj);
}

bool RenderQueue::debugIngameCheckSorted() {
    std::list<world::IngameObject*>::const_iterator iter = ingameList_.begin();
    std::list<world::IngameObject*>::const_iterator last = iter;
    ++iter;
    std::list<world::IngameObject*>::const_iterator end = ingameList_.end();

    for (unsigned int idx = 0; iter != end; ++iter, ++last, ++idx) {
        if (!renderPriorityComparator(*last, *iter)) {
            LOGARG_ERROR(LOGTYPE_UI, "unsorted list elements at %u", idx);
            return false;
        }
    }

    return true;
}

bool RenderQueue::debugIngameCheckInList(world::IngameObject* obj) {
    int idx = 0;
    std::list<world::IngameObject*>::const_iterator iter = ingameList_.begin();
    std::list<world::IngameObject*>::const_iterator end = ingameList_.end();

    for (; iter != end; ++iter) {
        if (*iter == obj) {
            LOGARG_ERROR(LOGTYPE_UI, "index in list: %i of %u", idx, ingameList_.size());
            return true;
        }
        ++idx;
    }

    return false;
}

world::IngameObject* RenderQueue::debugIngameGetByIndex(unsigned int idx) {
    std::list<world::IngameObject*>::const_iterator iter = ingameList_.begin();
    std::list<world::IngameObject*>::const_iterator end = ingameList_.end();

    for (unsigned int i = 0; i < idx; ++iter, ++i);

    return *iter;
}

void RenderQueue::remove(boost::shared_ptr<world::Sector> sector) {
    std::list<boost::shared_ptr<world::StaticItem> > staticList = sector->getStaticBlock()->getItemList();
    std::list<boost::shared_ptr<world::StaticItem> >::const_iterator it = staticList.begin();
    std::list<boost::shared_ptr<world::StaticItem> >::const_iterator end = staticList.end();

    for (; it != end; ++it) {
        ingameRemoveList_.push_back(*it);
    }

    for (unsigned int x = 0; x < 8; ++x) {
        for (unsigned int y = 0; y < 8; ++y) {
            ingameRemoveList_.push_back(sector->getMapBlock()->get(x, y));
        }
    }
}

void RenderQueue::remove(boost::shared_ptr<world::IngameObject> obj) {
    ingameRemoveList_.push_back(obj);
}


void RenderQueue::processRemoveList() {
    ingameRemoveList_.sort(boost::bind(&RenderQueue::renderPriorityComparatorSharedPtr, this, _1, _2));

    // the ingameRemoveList_ list is now in the same order as ingameList_
    // thus, we just need to iterate the list one time to remove all objects

    std::list<boost::shared_ptr<world::IngameObject> >::iterator deleteIter = ingameRemoveList_.begin();
    std::list<boost::shared_ptr<world::IngameObject> >::iterator deleteEnd = ingameRemoveList_.end();
    std::list<world::IngameObject*>::iterator ingameIter = ingameList_.begin();
    std::list<world::IngameObject*>::iterator ingameEnd = ingameList_.end();
    std::list<world::IngameObject*>::iterator deleteHelper;

    while(ingameIter != ingameEnd) {
        world::IngameObject* curDelete = (*deleteIter).get();

        if (*ingameIter == curDelete) {
            // mark item as deleted
            curDelete->addedToRenderQueue_ = false;

            deleteHelper = ingameIter;
            ++ingameIter;
            ingameList_.erase(deleteHelper);

            ++deleteIter;
            if (deleteIter == deleteEnd) {
                //LOG_DEBUG(LOGTYPE_UI, "RenderQueue::processRemoveList - list end reached");
                break;
            }
        } else {
            ++ingameIter;
        }
    }

    if (deleteIter != deleteEnd) {
        LOG_ERROR(LOGTYPE_UI, "RenderQueue::processRemoveList - ingameRemoveList_ end not reached");
        ingameRemoveList_.erase(ingameRemoveList_.begin(), deleteIter);
    } else {
        ingameRemoveList_.clear();
    }
}

void RenderQueue::removeImmediately(world::IngameObject* obj) {
    std::pair<
        std::list<world::IngameObject*>::iterator,
        std::list<world::IngameObject*>::iterator
    > eq = std::equal_range(ingameList_.begin(), ingameList_.end(), obj, boost::bind(&RenderQueue::renderPriorityComparator, this, _1, _2));

    std::list<world::IngameObject*>::iterator listItm = eq.first;

    bool erased = false;
    for (; listItm != eq.second; ++listItm) {
        if (*listItm == obj) {
            ingameList_.erase(listItm);
            erased = true;
            break;
        }
    }
}

bool RenderQueue::processAddList() {
    bool ret = false;
    boost::mutex::scoped_lock myLock(ingameAddListMutex_);
    if (!ingameAddList_.empty()) {
        ingameList_.insert(ingameList_.end(), ingameAddList_.begin(), ingameAddList_.end());
        ingameAddList_.clear();
        ret = true;
    }
    return ret;
}

void RenderQueue::prepareRender() {
    processRemoveList();
    bool requireSort = processAddList();


    std::list<world::IngameObject*>::iterator igIter = ingameList_.begin();
    std::list<world::IngameObject*>::iterator igEnd = ingameList_.end();

    for (; igIter != igEnd; ++igIter) {
        world::IngameObject* curObj = *igIter;
        // update rendering data (priority, vertex coordinates, texture, ...)
        if (!curObj->isRenderDataValid()) {
            curObj->updateRenderData();
            requireSort = true;
        }
    }

    if (requireSort) {
        sortIngame();
    }
}

void RenderQueue::sortIngame() {
    ingameList_.sort(boost::bind(&RenderQueue::renderPriorityComparator, this, _1, _2));
}

std::list<world::IngameObject*>::const_iterator RenderQueue::beginIngame() {
    return ingameList_.begin();
}

std::list<world::IngameObject*>::const_iterator RenderQueue::endIngame() {
    return ingameList_.end();
}

void RenderQueue::clear() {
    std::list<world::IngameObject*>::iterator igIter = ingameList_.begin();
    std::list<world::IngameObject*>::iterator igEnd = ingameList_.end();

    for (; igIter != igEnd; ++igIter) {
        world::IngameObject* curObj = *igIter;
        // update rendering data (priority, vertex coordinates, texture, ...)
        curObj->addedToRenderQueue_ = false;
    }

    ingameList_.clear();
}

}
}
