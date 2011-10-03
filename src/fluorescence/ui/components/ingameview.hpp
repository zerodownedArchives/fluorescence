#ifndef FLUO_UI_INGAMEVIEW_HPP
#define FLUO_UI_INGAMEVIEW_HPP

#include <boost/shared_ptr.hpp>

#include <list>

#include <ui/gumpelement.hpp>

namespace fluo {

namespace world {
class IngameObject;
class IngameObject;
}

namespace ui {

class IngameViewRenderer;

class IngameView : public GumpElement {
public:
    IngameView(CL_GUIComponent* parent, const CL_Rect& bounds);
    ~IngameView();

    void setCenterObject(boost::shared_ptr<world::IngameObject> obj);

    void setCenterTiles(float x, float y);

    int getCenterPixelX();
    int getCenterPixelY();

    unsigned int getWidth();
    unsigned int getHeight();

    void renderOneFrame(CL_GraphicContext& gc, const CL_Rect& clipRect);

    /// store all sectors this view needs (including some cache) in the list
    void getRequiredSectors(std::list<unsigned int>& list, unsigned int mapHeight, unsigned int cacheAdd);

    boost::shared_ptr<world::IngameObject> getFirstIngameObjectAt(unsigned int pixelX, unsigned int pixelY);

private:
    float centerTileX_;
    float centerTileY_;

    boost::shared_ptr<world::IngameObject> centerObject_;

    float getCenterTileX();
    float getCenterTileY();

    boost::shared_ptr<IngameViewRenderer> renderer_;

    bool onInputPressed(const CL_InputEvent& e);
    bool onInputReleased(const CL_InputEvent & e);
    bool onDoubleClick(const CL_InputEvent& e);
};

}
}

#endif
