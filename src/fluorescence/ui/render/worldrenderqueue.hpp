#ifndef FLUO_UI_WORLDRENDERQUEUE_HPP
#define FLUO_UI_WORLDRENDERQUEUE_HPP

#include <list>
#include <boost/shared_ptr.hpp>

#include <ui/render/renderqueue.hpp>

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

    virtual void preRender();
    virtual void postRender(bool renderingComplete);

    virtual boost::shared_ptr<world::IngameObject> getFirstObjectAt(int worldX, int worldY, bool getTopParent);

private:
    /// used to sort the objects according to their render priority. returns true, if a should be painted before b
    bool renderPriorityComparator(const boost::shared_ptr<world::IngameObject>& a, const boost::shared_ptr<world::IngameObject>& b);

    bool batchedComparator(const boost::shared_ptr<world::IngameObject>& a, const boost::shared_ptr<world::IngameObject>& b);
};

}
}

#endif
