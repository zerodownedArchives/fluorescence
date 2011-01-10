#ifndef UOME_UI_INGAMEWINDOW_HPP
#define UOME_UI_INGAMEWINDOW_HPP

#include <boost/shared_ptr.hpp>

#include "uiobject.hpp"

namespace uome {
namespace ui {

class IngameWindowRenderer;

class IngameWindow : public UiObject {
public:
    IngameWindow(CL_GUIManager* manager);

    unsigned int getCenterTileX();
    unsigned int getCenterTileY();

    void setCenterTiles(unsigned int x, unsigned int y);

    int getCenterPixelX();
    int getCenterPixelY();

    unsigned int getWidth();
    unsigned int getHeight();

    void renderOneFrame(CL_GraphicContext& gc, const CL_Rect& clipRect);

private:
    unsigned int centerTileX_;
    unsigned int centerTileY_;

    boost::shared_ptr<IngameWindowRenderer> renderer_;
};

}
}

#endif
