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

    void clear();
    void sort();

    virtual void preRender() = 0;
    virtual void postRender() = 0;
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
    void onObjectWorldPriorityChanged();
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
    bool worldPriorityChanged_;
    bool forceRepaint_;

    bool gumpChanged_;

private:
    std::list<boost::shared_ptr<world::IngameObject> > objectList_;

    // ingameobjects might be added to or deleted from the render queue asynchronously. thus, we keep an extra list for added/deleted items
    std::list<boost::shared_ptr<world::IngameObject> > addList_;
    boost::mutex addListMutex_;

    SortFunction sortFunction_;


    bool debugIngameCheckSorted();
    bool debugIngameCheckInList(boost::shared_ptr<world::IngameObject> obj);
    boost::shared_ptr<world::IngameObject> debugIngameGetByIndex(unsigned int idx);

    void add(boost::shared_ptr<world::IngameObject> obj);
    void remove(boost::shared_ptr<world::IngameObject> obj);

    friend void world::IngameObject::addToRenderQueue(boost::shared_ptr<RenderQueue> rq);
    friend void world::IngameObject ::removeFromRenderQueue(boost::shared_ptr<RenderQueue> rq);
};

}
}

#endif
