#ifndef UOME_UI_RENDERQUEUE_HPP
#define UOME_UI_RENDERQUEUE_HPP

#include <list>

#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>

namespace uome {

namespace world {
    class IngameObject;
    class Sector;
}

namespace ui {

class UiObject;

class RenderQueue {
public:
    RenderQueue();
    ~RenderQueue();

    void add(world::IngameObject* obj);

    /// do not use unless absolutely necessary
    void removeImmediately(world::IngameObject* obj);

    void remove(boost::shared_ptr<world::IngameObject> obj);
    void remove(boost::shared_ptr<world::Sector> sector);

    /// batch delete function for sectors. way faster than deleting each item individually
    void removeSector(world::Sector* sector);

    /// Calling this function will make the client re-sort the queue at the next rendering
    void requireIngameSort();
    void prepareRender(unsigned int elapsedMillis);

    std::list<world::IngameObject*>::const_iterator beginIngame();
    std::list<world::IngameObject*>::const_iterator endIngame();

    unsigned int size() { return ingameList_.size(); }

    void clear();

    boost::shared_ptr<world::IngameObject> getFirstIngameObjectAt(int worldX, int worldY, bool getTopParent);

private:
    /* I'm aware of the fact that, according to good programming standards, this should be a list of weak_ptr
     * I experimented with that quite a bit, but implementing this list with weak pointers has an incredibly heavy
     * performance impact (~ 6x times slower on my computer). Thus, I decided to stick to plain pointers here.
     * You can find the weak pointer experiments in the git tree, if I managed to use git correctly ;)
     */
    std::list<world::IngameObject*> ingameList_;

    void sortIngame();
    /// used to sort the objects according to their render priority. returns true, if a should be painted before b
    bool renderPriorityComparator(const world::IngameObject* a, const world::IngameObject* b);
    bool renderPriorityComparatorSharedPtr(const boost::shared_ptr<world::IngameObject>& a, const boost::shared_ptr<world::IngameObject>& b);

    void processRemoveList();
    bool processAddList();

    bool debugIngameCheckSorted();
    bool debugIngameCheckInList(world::IngameObject* obj);
    world::IngameObject* debugIngameGetByIndex(unsigned int idx);

    // ingameobjects might be added to or deleted from the render queue asynchronously. thus, we keep an extra list for added/deleted items
    std::list<world::IngameObject*> ingameAddList_;
    boost::mutex ingameAddListMutex_;

    // removing items from the queue is usually done in prepare render. but the objects have to stay alive long enough
    // to remove them properly, so for delayed removal, we take ownership.
    // for immediate (emergency only!) removal, there is removeImmediately(IngameObject*)
    std::list<boost::shared_ptr<world::IngameObject> > ingameRemoveList_;
};

}
}

#endif
