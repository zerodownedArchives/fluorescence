#ifndef FLUO_UI_CONTAINERVIEW_HPP
#define FLUO_UI_CONTAINERVIEW_HPP

#include <boost/shared_ptr.hpp>

#include <list>

#include <ui/gumpelement.hpp>

namespace fluo {

namespace world {
class IngameObject;
class DynamicItem;
}

namespace ui {

class ContainerRenderer;
class Texture;

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

    void setBackgroundGumpId(unsigned int gumpId);
    void setContainerObject(boost::shared_ptr<world::DynamicItem> cont);

    boost::shared_ptr<ui::Texture> getBackgroundTexture();

private:
    boost::shared_ptr<ContainerRenderer> renderer_;

    bool onInputPressed(const CL_InputEvent& e);
    bool onInputReleased(const CL_InputEvent & e);
    bool onDoubleClick(const CL_InputEvent& e);

    boost::shared_ptr<ui::Texture> backgroundTexture_;
    bool sizeAdjusted;
    boost::shared_ptr<world::DynamicItem> containerObject_;
};

}
}
}

#endif
