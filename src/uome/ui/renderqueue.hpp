#ifndef UOME_UI_RENDERQUEUE_HPP
#define UOME_UI_RENDERQUEUE_HPP

#include <list>
#include <boost/thread/mutex.hpp>

namespace uome {

namespace world {
    class IngameObject;
    class Sector;
}

namespace ui {

class RenderQueue {
public:
    RenderQueue();
    ~RenderQueue();

    void add(world::IngameObject* obj);
    void remove(world::IngameObject* obj);

    /// batch delete function for sectors. way faster than deleting each item individually
    void removeSector(world::Sector* sector);

    /// Calling this function will make the client re-sort the queue at the next rendering
    void requireSort();
    void prepareRender();

    std::list<world::IngameObject*>::const_iterator beginIngame();
    std::list<world::IngameObject*>::const_iterator endIngame();

    /// used to sort the objects according to their render priority. returns true, if a should be painted before b
    bool renderPriorityComparator(const world::IngameObject* a, const world::IngameObject* b);

    unsigned int size() { return ingameList_.size(); }

private:
    /* I'm aware of the fact that, according to good programming standards, this should be a list of weak_ptr
     * I experimented with that quite a bit, but implementing this list with weak pointers has an incredibly heavy
     * performance impact (~ 6x times slower on my computer). Thus, I decided to stick to plain pointers here.
     * You can find the weak pointer experiments in the git tree, if I managed to use git correctly ;)
     */
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
