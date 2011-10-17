#ifndef FLUO_UI_GUMPRENDERQUEUE_HPP
#define FLUO_UI_GUMPRENDERQUEUE_HPP

#include <list>
#include <boost/shared_ptr.hpp>

#include <ui/render/renderqueue.hpp>

namespace fluo {

namespace world {
    class IngameObject;
    class Sector;
}

namespace ui {

class GumpRenderQueue : public RenderQueue {
public:
    GumpRenderQueue();
    ~GumpRenderQueue();

    virtual void preRender();
    virtual void postRender();

    virtual boost::shared_ptr<world::IngameObject> getFirstObjectAt(int worldX, int worldY, bool getTopParent);

private:
    /// used to sort the objects according to their layer
    bool renderPriorityComparator(const boost::shared_ptr<world::IngameObject>& a, const boost::shared_ptr<world::IngameObject>& b);

    std::vector<int> layerPriorities_;
};

}
}

#endif
