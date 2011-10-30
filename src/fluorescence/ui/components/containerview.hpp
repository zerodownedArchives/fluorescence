#ifndef FLUO_UI_CONTAINERVIEW_HPP
#define FLUO_UI_CONTAINERVIEW_HPP

#include <boost/shared_ptr.hpp>

#include <list>

#include <ui/gumpelement.hpp>

namespace fluo {

namespace world {
class IngameObject;
}

namespace ui {

class ContainerRenderer;

namespace components {

class ContainerView : public GumpElement {
public:
    ContainerView(CL_GUIComponent* parent, const CL_Rect& bounds);
    ~ContainerView();

    unsigned int getWidth();
    unsigned int getHeight();

    void renderOneFrame(CL_GraphicContext& gc, const CL_Rect& clipRect);

    void addObject(boost::shared_ptr<world::IngameObject> obj);
    void removeObject(boost::shared_ptr<world::IngameObject> obj);

private:
    boost::shared_ptr<ContainerRenderer> renderer_;

    bool onInputPressed(const CL_InputEvent& e);
    bool onInputReleased(const CL_InputEvent & e);
    bool onDoubleClick(const CL_InputEvent& e);
};

}
}
}

#endif
