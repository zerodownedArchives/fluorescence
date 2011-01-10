#ifndef UOME_UI_INGAMEVIEW_HPP
#define UOME_UI_INGAMEVIEW_HPP

#include <boost/shared_ptr.hpp>

#include <list>

#include "gumpmenu.hpp"

namespace uome {
namespace ui {

class IngameViewRenderer;

class IngameView : public GumpMenu {
public:
    IngameView(const CL_Rect& bounds);

    unsigned int getCenterTileX();
    unsigned int getCenterTileY();

    void setCenterTiles(unsigned int x, unsigned int y);

    int getCenterPixelX();
    int getCenterPixelY();

    unsigned int getWidth();
    unsigned int getHeight();

    void renderOneFrame(CL_GraphicContext& gc, const CL_Rect& clipRect);

    /// store all sectors this view needs (including some cache) in the list
    void getRequiredSectors(std::list<unsigned int>& list, unsigned int mapHeight);

private:
    unsigned int centerTileX_;
    unsigned int centerTileY_;

    boost::shared_ptr<IngameViewRenderer> renderer_;
};

}
}

#endif
