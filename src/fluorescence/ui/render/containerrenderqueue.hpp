#ifndef FLUO_UI_CONTAINERRENDERQUEUE_HPP
#define FLUO_UI_CONTAINERRENDERQUEUE_HPP

#include <list>
#include <boost/shared_ptr.hpp>

#include <ui/render/renderqueue.hpp>

namespace fluo {

namespace world {
    class IngameObject;
}

namespace ui {

class ContainerRenderQueue : public RenderQueue {
public:
    ContainerRenderQueue();
    ~ContainerRenderQueue();

    virtual void preRender();
    virtual void postRender(bool renderingComplete);

    virtual boost::shared_ptr<world::IngameObject> getFirstObjectAt(int worldX, int worldY, bool getTopParent);

private:
    /// used to sort the objects according to their layer
    bool renderPriorityComparator(const boost::shared_ptr<world::IngameObject>& a, const boost::shared_ptr<world::IngameObject>& b);
};

}
}

#endif
