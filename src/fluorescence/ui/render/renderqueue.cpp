/*
 * fluorescence is a free, customizable Ultima Online client.
 * Copyright (C) 2011-2012, http://fluorescence-client.org

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */



#include "renderqueue.hpp"

#include <misc/log.hpp>
#include <world/ingameobject.hpp>
#include <world/sector.hpp>

namespace fluo {
namespace ui {

RenderQueue::RenderQueue(SortFunction sortFunction) : sortFunction_(sortFunction) {
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

void RenderQueue::remove(const boost::shared_ptr<world::IngameObject>& obj) {
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

void RenderQueue::add(const boost::shared_ptr<world::IngameObject>& obj) {
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

    if (notifyFunction_) {
        notifyFunction_();
    }
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

void RenderQueue::setNotifyFunction(NotifyFunction func) {
    notifyFunction_ = func;
}

}
}
