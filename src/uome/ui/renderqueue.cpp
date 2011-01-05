
#include "renderqueue.hpp"

#include <boost/bind.hpp>

#include <misc/logger.hpp>
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
            printf("unsorted list elements at %u\n", idx);
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
            printf("index in list: %i of %u\n", idx, ingameList_.size());
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


void RenderQueue::remove(world::IngameObject* obj) {
    //printf("rq count before: %u\n", ingameList_.size());
    //printf("counted: %u\n", std::count(ingameList_.begin(), ingameList_.end(), obj));

    //if (!std::binary_search(ingameList_.begin(), ingameList_.end(), obj, boost::bind(&RenderQueue::renderPriorityComparator, this, _1, _2))) {
        //printf("item not even in the list\n");
        //if (realCheckInList(obj)) {
            //printf("but real check says it is\n");
        //} else {
            //printf("really not\n");
        //}

        //if (realCheckSorted()) {
            //printf("list is sorted\n");
        //} else {
            //printf("list is not sorted! wtf?\n");
        //}

        //int i = 0;
        //(void)i;
    //}

    //ingameList_.remove(*deleteIter);

    //std::list<world::IngameObject*>::iterator listItm =
        //std::lower_bound(ingameList_.begin(), ingameList_.end(), obj, boost::bind(&RenderQueue::renderPriorityComparator, this, _1, _2));

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
    //if (!erased) {
        //int i = 0;
        //(void)i;
    //}

    //printf ("Item successfully erased: %i\n", erased);

    //if (*listItm == obj) {
        //ingameList_.erase(listItm);
    //} else {
        //printf("listItm iterator != obj\n");
    //}

    //boost::mutex::scoped_lock myLock(ingameDeleteListMutex_);
    //ingameDeleteList_.push_back(obj);
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

    sort();

    {
        //boost::mutex::scoped_lock myLock(ingameDeleteListMutex_);
        //for (std::list<world::IngameObject*>::iterator deleteIter = ingameDeleteList_.begin(); deleteIter != ingameDeleteList_.end(); ++deleteIter) {
            //printf("rq count before: %u\n", ingameList_.size());
            //printf("counted: %u\n", std::count(ingameList_.begin(), ingameList_.end(), obj));
            //std::pair<
            //std::list<world::IngameObject*>::iterator,
            //std::list<world::IngameObject*>::iterator
            //> eq = std::equal_range(ingameList_.begin(), ingameList_.end(), obj, boost::bind(&RenderQueue::renderPriorityComparator, this, _1, _2));

            //if (!std::binary_search(ingameList_.begin(), ingameList_.end(), *deleteIter, boost::bind(&RenderQueue::renderPriorityComparator, this, _1, _2))) {
                //printf("item not even in the list\n");
                //if (realCheckInList(*deleteIter)) {
                    //printf("but real check says it is\n");
                //} else {
                    //printf("really not\n");
                //}

                //if (realCheckSorted()) {
                    //printf("list is sorted\n");
                //} else {
                    //printf("list is not sorted! wtf?\n");
                //}

                //int i = 0;
                //(void)i;
                //continue;
            //}

            //std::list<world::IngameObject*>::iterator listItm =
                //std::lower_bound(ingameList_.begin(), ingameList_.end(), *deleteIter, boost::bind(&RenderQueue::renderPriorityComparator, this, _1, _2));
            ////ingameList_.remove(*deleteIter);

            //if (listItm != ingameList_.end()) {
                //ingameList_.erase(listItm);
            //} else {
                //printf("listItm iterator == NULL!!!!!\n");
            //}
        //}

        //ingameDeleteList_.clear();
    }
}

void RenderQueue::sort() {
    if (!sorted_) {
        ingameList_.sort(boost::bind(&RenderQueue::renderPriorityComparator, this, _1, _2));
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
