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


#ifndef FLUO_UI_RENDERQUEUE_HPP
#define FLUO_UI_RENDERQUEUE_HPP

#include <list>
#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/function.hpp>
#include <boost/enable_shared_from_this.hpp>

#include <world/ingameobject.hpp>

namespace fluo {

namespace world {
    class Sector;
}

namespace ui {

class RenderQueue : public boost::enable_shared_from_this<RenderQueue> {
public:
    typedef std::list<boost::shared_ptr<world::IngameObject> >::iterator iterator;
    typedef std::list<boost::shared_ptr<world::IngameObject> >::const_iterator const_iterator;
    typedef std::list<boost::shared_ptr<world::IngameObject> >::reverse_iterator reverse_iterator;
    typedef std::list<boost::shared_ptr<world::IngameObject> >::const_reverse_iterator const_reverse_iterator;

    typedef boost::function<bool (const boost::shared_ptr<world::IngameObject>&, const boost::shared_ptr<world::IngameObject>&)> SortFunction;


    RenderQueue(SortFunction sortFunction);
    RenderQueue(SortFunction sortFunction, SortFunction batchedSortFunction_);

    void clear();
    void sort();

    virtual void preRender() = 0;
    virtual void postRender(bool renderingComplete) = 0;
    virtual boost::shared_ptr<world::IngameObject> getFirstObjectAt(int worldX, int worldY, bool getTopParent) = 0;

    unsigned int size() const;

    RenderQueue::iterator begin();
    RenderQueue::const_iterator begin() const;

    RenderQueue::iterator end();
    RenderQueue::const_iterator end() const;

    RenderQueue::reverse_iterator rbegin();
    RenderQueue::const_reverse_iterator rbegin() const;

    RenderQueue::reverse_iterator rend();
    RenderQueue::const_reverse_iterator rend() const;


    void onObjectWorldTextureChanged();
    void onObjectWorldCoordinatesChanged();
    void onObjectWorldDepthChanged();
    void onGumpChanged();
    void forceRepaint();

    bool requireWorldRepaint() const;
    void resetWorldRepaintIndicators();

    bool requireGumpRepaint() const;
    void resetGumpRepaintIndicators();

protected:
    void processRemoveList();
    bool processAddList();

    // items to remove are collected here before removal, because batch remove is more efficient
    std::list<boost::shared_ptr<world::IngameObject> > removeList_;

    bool worldTextureChanged_;
    bool worldCoordinatesChanged_;
    bool worldDepthChanged_;
    bool forceRepaint_;

    bool gumpChanged_;

    bool debugIngameCheckSorted();
    bool debugIngameCheckInList(boost::shared_ptr<world::IngameObject> obj);
    boost::shared_ptr<world::IngameObject> debugIngameGetByIndex(unsigned int idx);

    // ingameobjects might be added to or deleted from the render queue asynchronously. thus, we keep an extra list for added/deleted items
    std::list<boost::shared_ptr<world::IngameObject> > addList_;
    boost::mutex addListMutex_;

private:
    std::list<boost::shared_ptr<world::IngameObject> > objectList_;

    SortFunction sortFunction_;

    void add(const boost::shared_ptr<world::IngameObject>& obj);
    void remove(const boost::shared_ptr<world::IngameObject>& obj);

    friend void world::IngameObject::addToRenderQueue(const boost::shared_ptr<RenderQueue>& rq);
    friend void world::IngameObject ::removeFromRenderQueue(const boost::shared_ptr<RenderQueue>& rq);
};

}
}

#endif
