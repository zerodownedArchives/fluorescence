#ifndef FLUO_UI_GUMPVIEW_HPP
#define FLUO_UI_GUMPVIEW_HPP

#include <boost/shared_ptr.hpp>

#include <list>

#include <ui/gumpelement.hpp>

namespace fluo {

namespace world {
class IngameObject;
}

namespace ui {

class GumpRenderer;

class GumpView : public GumpElement {
public:
    GumpView(CL_GUIComponent* parent, const CL_Rect& bounds);
    ~GumpView();

    unsigned int getWidth();
    unsigned int getHeight();

    void renderOneFrame(CL_GraphicContext& gc, const CL_Rect& clipRect);

    void addObject(boost::shared_ptr<world::IngameObject> obj);
    void removeObject(boost::shared_ptr<world::IngameObject> obj);


    boost::shared_ptr<world::IngameObject> getFirstIngameObjectAt(unsigned int pixelX, unsigned int pixelY);

private:
    boost::shared_ptr<GumpRenderer> renderer_;

    bool onInputPressed(const CL_InputEvent& e);
    bool onInputReleased(const CL_InputEvent & e);
    bool onDoubleClick(const CL_InputEvent& e);
};

}
}

#endif
