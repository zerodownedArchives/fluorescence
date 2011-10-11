#ifndef FLUO_UI_INGAMERENDERQUEUE_HPP
#define FLUO_UI_INGAMERENDERQUEUE_HPP

#include <list>
#include <boost/shared_ptr.hpp>

#include "renderqueue.hpp"

namespace fluo {

namespace world {
    class IngameObject;
    class Sector;
}

namespace ui {

class IngameRenderQueue : public RenderQueue {
public:
    IngameRenderQueue();
    ~IngameRenderQueue();

    /// batch delete function for sectors. way faster than deleting each item individually
    void remove(boost::shared_ptr<world::Sector> sector);

    /// Calling this function will make the client re-sort the queue at the next rendering
    void requireIngameSort();
    void prepareRender(unsigned int elapsedMillis);

    boost::shared_ptr<world::IngameObject> getFirstIngameObjectAt(int worldX, int worldY, bool getTopParent);

private:
    void sortIngame();
    /// used to sort the objects according to their render priority. returns true, if a should be painted before b
    bool renderPriorityComparator(const boost::shared_ptr<world::IngameObject>& a, const boost::shared_ptr<world::IngameObject>& b);

};

}
}

#endif
