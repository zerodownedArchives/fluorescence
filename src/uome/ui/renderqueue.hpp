#ifndef UOME_UI_RENDERQUEUE_HPP
#define UOME_UI_RENDERQUEUE_HPP

#include <list>

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
    void sort();

    std::list<world::IngameObject*>::iterator beginIngame();
    std::list<world::IngameObject*>::iterator endIngame();

private:
    bool sorted_;
    std::list<world::IngameObject*> ingameList_;
};

}
}

#endif
