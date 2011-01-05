
#include "renderqueue.hpp"

#include <boost/bind.hpp>

#include <misc/logger.hpp>

#include <world/ingameobject.hpp>
#include <world/sector.hpp>

namespace uome {
namespace ui {

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

RenderQueue::RenderQueue() :
        sorted_(true) {
}

RenderQueue::~RenderQueue() {
}

void RenderQueue::add(world::IngameObject* obj) {
    boost::mutex::scoped_lock myLock(ingameAddListMutex_);
    ingameAddList_.push_back(obj);
}

bool RenderQueue::realCheckSorted() {
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

bool RenderQueue::realCheckInList(world::IngameObject* obj) {
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

world::IngameObject* RenderQueue::getByIndex(unsigned int idx) {
    std::list<world::IngameObject*>::const_iterator iter = ingameList_.begin();
    std::list<world::IngameObject*>::const_iterator end = ingameList_.end();

    for (unsigned int i = 0; i < idx; ++iter, ++i);

    return *iter;
}

void RenderQueue::removeSector(world::Sector* sector) {
    std::list<world::IngameObject*> allSectorObjects;

    std::list<boost::shared_ptr<world::StaticItem> > staticList = sector->getStaticBlock()->getItemList();
    std::list<boost::shared_ptr<world::StaticItem> >::const_iterator it = staticList.begin();
    std::list<boost::shared_ptr<world::StaticItem> >::const_iterator end = staticList.end();

    for (; it != end; ++it) {
        allSectorObjects.push_back((*it).get());
    }

    for (unsigned int x = 0; x < 8; ++x) {
        for (unsigned int y = 0; y < 8; ++y) {
            allSectorObjects.push_back(sector->getMapBlock()->get(x, y).get());
        }
    }

    allSectorObjects.sort(boost::bind(&RenderQueue::renderPriorityComparator, this, _1, _2));


    // the allSectorObjects list is now in the same order as ingameList_
    // thus, we just need to iterate the list one time to remove all objects

    std::list<world::IngameObject*>::const_iterator deleteIter = allSectorObjects.begin();
    std::list<world::IngameObject*>::const_iterator deleteEnd = allSectorObjects.end();
    std::list<world::IngameObject*>::iterator ingameIter = ingameList_.begin();
    std::list<world::IngameObject*>::iterator ingameEnd = ingameList_.end();
    std::list<world::IngameObject*>::iterator deleteHelper;

    while(ingameIter != ingameEnd) {
        if (*ingameIter == *deleteIter) {
            // mark item as deleted
            (*deleteIter)->addedToRenderQueue_ = false;

            deleteHelper = ingameIter;
            ++ingameIter;
            ingameList_.erase(deleteHelper);

            ++deleteIter;
            if (deleteIter == deleteEnd) {
                //LOG_DEBUG(LOGTYPE_UI, "RenderQueue::removeSector - sector list end reached");
                break;
            }
        } else {
            ++ingameIter;
        }
    }

    if (deleteIter != deleteEnd) {
        LOG_ERROR(LOGTYPE_UI, "RenderQueue::removeSector - sector list end not reached");
    }
}

void RenderQueue::remove(world::IngameObject* obj) {
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

    std::list<world::IngameObject*>::iterator igIter = ingameList_.begin();
    std::list<world::IngameObject*>::iterator igEnd = ingameList_.end();

    for (; igIter != igEnd; ++igIter) {
        world::IngameObject* curObj = *igIter;
        // update rendering data (priority, vertex coordinates, texture, ...)
        if (!curObj->isRenderDataValid()) {
            curObj->updateRenderData();
        }
    }

    sort();
}

void RenderQueue::sort() {
    if (!sorted_) {
        ingameList_.sort(boost::bind(&RenderQueue::renderPriorityComparator, this, _1, _2));
        sorted_ = true;
    }
}

std::list<world::IngameObject*>::const_iterator RenderQueue::beginIngame() {
    return ingameList_.begin();
}

std::list<world::IngameObject*>::const_iterator RenderQueue::endIngame() {
    return ingameList_.end();
}

}
}
