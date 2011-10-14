#ifndef FLUO_UI_WORLDRENDERQUEUE_HPP
#define FLUO_UI_WORLDRENDERQUEUE_HPP

#include <list>
#include <boost/shared_ptr.hpp>

#include <ui/renderqueue.hpp>

namespace fluo {

namespace world {
    class IngameObject;
    class Sector;
}

namespace ui {

class WorldRenderQueue : public RenderQueue {
public:
    WorldRenderQueue();
    ~WorldRenderQueue();

    /// Calling this function will make the client re-sort the queue at the next rendering
    void requireIngameSort();
    virtual void prepareRender(unsigned int elapsedMillis);

    virtual boost::shared_ptr<world::IngameObject> getFirstObjectAt(int worldX, int worldY, bool getTopParent);

private:
    void sortIngame();
    /// used to sort the objects according to their render priority. returns true, if a should be painted before b
    bool renderPriorityComparator(const boost::shared_ptr<world::IngameObject>& a, const boost::shared_ptr<world::IngameObject>& b);

};

}
}

#endif
