#ifndef UOME_UI_RENDERQUEUE_HPP
#define UOME_UI_RENDERQUEUE_HPP

#include <list>
#include <boost/thread/mutex.hpp>

namespace uome {

namespace world {
    class IngameObject;
}

namespace ui {

class RenderQueue {
public:
    RenderQueue();
    ~RenderQueue();

    void add(world::IngameObject* obj);
    void remove(world::IngameObject* obj);

    /// Calling this function will make the client re-sort the queue at the next rendering
    void requireSort();
    void prepareRender();

    std::list<world::IngameObject*>::iterator beginIngame();
    std::list<world::IngameObject*>::iterator endIngame();

    /// used to sort the objects according to their render priority. returns true, if a should be painted before b
    bool renderPriorityComparator(const world::IngameObject* a, const world::IngameObject* b);

    unsigned int size() { return ingameList_.size(); }

private:
    std::list<world::IngameObject*> ingameList_;

    bool sorted_;
    void sort();

    bool realCheckSorted();
    bool realCheckInList(world::IngameObject* obj);
    world::IngameObject* getByIndex(unsigned int idx);

    // ingameobjects might be added to or deleted from the render queue asynchronously. thus, we keep an extra list for added/deleted items
    std::list<world::IngameObject*> ingameAddList_;
    boost::mutex ingameAddListMutex_;

    std::list<world::IngameObject*> ingameDeleteList_;
    boost::mutex ingameDeleteListMutex_;
};

}
}

#endif
