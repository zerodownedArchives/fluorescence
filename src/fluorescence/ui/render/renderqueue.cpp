
#include "renderqueue.hpp"

#include <misc/log.hpp>
#include <world/ingameobject.hpp>
#include <world/sector.hpp>

namespace fluo {
namespace ui {

RenderQueue::RenderQueue(SortFunction sortFunction) : sortFunction_(sortFunction), minRenderDepth_(0xFFFFFFFFFFFFFFFFu), maxRenderDepth_(0) {
}

RenderQueue::iterator RenderQueue::begin() {
    return objectList_.begin();
}

RenderQueue::const_iterator RenderQueue::begin() const {
    return objectList_.begin();
}

RenderQueue::iterator RenderQueue::end() {
    return objectList_.end();
}

RenderQueue::const_iterator RenderQueue::end() const {
    return objectList_.end();
}

RenderQueue::reverse_iterator RenderQueue::rbegin() {
    return objectList_.rbegin();
}

RenderQueue::const_reverse_iterator RenderQueue::rbegin() const {
    return objectList_.rbegin();
}

RenderQueue::reverse_iterator RenderQueue::rend() {
    return objectList_.rend();
}

RenderQueue::const_reverse_iterator RenderQueue::rend() const {
    return objectList_.rend();
}

void RenderQueue::clear() {
    boost::shared_ptr<RenderQueue> sharedThis = shared_from_this();

    RenderQueue::iterator iter = begin();
    RenderQueue::iterator iterEnd = end();

    for (; iter != iterEnd; ++iter) {
        (*iter)->removeFromRenderQueue(sharedThis);
    }

    objectList_.clear();

    // all items are in the delete list now
    removeList_.clear();

    forceRepaint();
}

bool RenderQueue::processAddList() {
    bool ret = false;
    boost::mutex::scoped_lock myLock(addListMutex_);
    if (!addList_.empty()) {
        objectList_.insert(objectList_.end(), addList_.begin(), addList_.end());
        addList_.clear();
        ret = true;
    }
    return ret;
}

void RenderQueue::remove(boost::shared_ptr<world::IngameObject> obj) {
    removeList_.push_back(obj);
    forceRepaint();
}

void RenderQueue::processRemoveList() {
    removeList_.sort(sortFunction_);

    // the ingameRemoveList_ list is now in the same order as ingameList_
    // thus, we just need to iterate the list one time to remove all objects

    std::list<boost::shared_ptr<world::IngameObject> >::iterator deleteIter = removeList_.begin();
    std::list<boost::shared_ptr<world::IngameObject> >::iterator deleteEnd = removeList_.end();
    std::list<boost::shared_ptr<world::IngameObject> >::iterator objIter = objectList_.begin();
    std::list<boost::shared_ptr<world::IngameObject> >::iterator objEnd = objectList_.end();
    std::list<boost::shared_ptr<world::IngameObject> >::iterator deleteHelper;

    boost::shared_ptr<RenderQueue> sharedThis = shared_from_this();

    while(objIter != objEnd) {
        boost::shared_ptr<world::IngameObject> curDelete = *deleteIter;

        if (*objIter == curDelete) {
            deleteHelper = objIter;
            ++objIter;
            objectList_.erase(deleteHelper);

            ++deleteIter;
            if (deleteIter == deleteEnd) {
                //LOG_DEBUG(LOGTYPE_UI, "RenderQueue::processRemoveList - list end reached");
                break;
            }
        } else {
            ++objIter;
        }
    }

    if (deleteIter != deleteEnd) {
        LOG_WARN << "RenderQueue::processRemoveList - removeList_ end not reached" << std::endl;

        LOG_WARN << "List size: " << objectList_.size() << std::endl;
        debugIngameCheckSorted();

        for (; deleteIter != deleteEnd; ++deleteIter) {
            objectList_.remove(*deleteIter);
        }
    }

    removeList_.clear();
}

void RenderQueue::add(boost::shared_ptr<world::IngameObject> obj) {
    boost::mutex::scoped_lock myLock(addListMutex_);
    addList_.push_back(obj);

    forceRepaint();
}

unsigned int RenderQueue::size() const {
    return objectList_.size();
}

void RenderQueue::sort() {
    objectList_.sort(sortFunction_);
}


bool RenderQueue::debugIngameCheckSorted() {
    std::list<boost::shared_ptr<world::IngameObject> >::const_iterator iter = objectList_.begin();
    std::list<boost::shared_ptr<world::IngameObject> >::const_iterator last = iter;
    ++iter;
    std::list<boost::shared_ptr<world::IngameObject> >::const_iterator end = objectList_.end();

    for (unsigned int idx = 0; iter != end; ++iter, ++last, ++idx) {
        if (!sortFunction_(*last, *iter)) {
            LOG_ERROR << "RenderQueue: unsorted list elements at " << idx << std::endl;
            LOG_ERROR << "Prio last: " << std::endl;
            (*last)->printRenderDepth();
            (*last)->onClick();
            LOG_ERROR << "Prio iter: " << std::endl;
            (*iter)->printRenderDepth();
            return false;
        }
    }

    return true;
}

bool RenderQueue::debugIngameCheckInList(boost::shared_ptr<world::IngameObject> obj) {
    unsigned int idx = 0;
    std::list<boost::shared_ptr<world::IngameObject> >::const_iterator iter = objectList_.begin();
    std::list<boost::shared_ptr<world::IngameObject> >::const_iterator end = objectList_.end();

    for (; iter != end; ++iter) {
        if (*iter == obj) {
            LOG_ERROR << "RenderQueue: index in list: " << idx << " of " << objectList_.size() << std::endl;
            return true;
        }
        ++idx;
    }

    return false;
}

boost::shared_ptr<world::IngameObject> RenderQueue::debugIngameGetByIndex(unsigned int idx) {
    std::list<boost::shared_ptr<world::IngameObject> >::const_iterator iter = objectList_.begin();
    std::list<boost::shared_ptr<world::IngameObject> >::const_iterator end = objectList_.end();

    for (unsigned int i = 0; i < idx; ++iter, ++i);

    boost::shared_ptr<world::IngameObject> ret(*iter);
    return ret;
}

void RenderQueue::onObjectWorldTextureChanged() {
    worldTextureChanged_ = true;
}

void RenderQueue::onObjectWorldCoordinatesChanged() {
    worldCoordinatesChanged_ = true;
}

void RenderQueue::onObjectWorldDepthChanged() {
    worldDepthChanged_ = true;
}

void RenderQueue::onGumpChanged() {
    gumpChanged_ = true;
}

void RenderQueue::forceRepaint() {
    forceRepaint_ = true;
}

bool RenderQueue::requireWorldRepaint() const {
    return worldTextureChanged_ || worldCoordinatesChanged_ || worldDepthChanged_ || forceRepaint_;
}

void RenderQueue::resetWorldRepaintIndicators() {
    worldTextureChanged_ = false;
    worldCoordinatesChanged_ = false;
    worldDepthChanged_ = false;
    forceRepaint_ = false;
}

bool RenderQueue::requireGumpRepaint() const {
    return gumpChanged_ || forceRepaint_;
}

void RenderQueue::resetGumpRepaintIndicators() {
    gumpChanged_ = false;
    forceRepaint_ = false;
}

void RenderQueue::updateMinMaxRenderDepth() {
    unsigned long long cur;

    minRenderDepth_ = 0xFFFFFFFFFFFFFFFFu;
    maxRenderDepth_ = 0;
    RenderQueue::iterator iter = objectList_.begin();
    RenderQueue::iterator end = objectList_.end();

    for (; iter != end; ++iter) {
        cur = (*iter)->getRenderDepth();
        if (cur == 0) {
            continue;
        } else if (cur < minRenderDepth_) {
            minRenderDepth_ = cur;
        } else if (cur > maxRenderDepth_) {
            maxRenderDepth_ = cur;
        }
    }

    boost::mutex::scoped_lock myLock(addListMutex_);

    iter = addList_.begin();
    end = addList_.end();

    for (; iter != end; ++iter) {
        cur = (*iter)->getRenderDepth();
        if (cur == 0) {
            continue;
        } else if (cur < minRenderDepth_) {
            minRenderDepth_ = cur;
        } else if (cur > maxRenderDepth_) {
            maxRenderDepth_ = cur;
        }
    }
}

unsigned long long RenderQueue::getMinRenderDepth() const {
    return minRenderDepth_;
}

unsigned long long RenderQueue::getMaxRenderDepth() const {
    return maxRenderDepth_;
}

}
}
