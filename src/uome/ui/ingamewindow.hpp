#ifndef UOME_UI_INGAMEWINDOW_HPP
#define UOME_UI_INGAMEWINDOW_HPP


#include <boost/shared_ptr.hpp>

namespace uome {
namespace ui {

class IngameWindowRenderer;

class IngameWindow {
public:
    IngameWindow(unsigned int width, unsigned int height);

    unsigned int getCenterTileX();
    unsigned int getCenterTileY();

    void setCenterTiles(unsigned int x, unsigned int y);

    int getCenterPixelX();
    int getCenterPixelY();

    unsigned int getWidth();
    unsigned int getHeight();

    void setWidth(unsigned int width);
    void setHeight(unsigned int height);

    void renderOneFrame();

private:
    unsigned int centerTileX_;
    unsigned int centerTileY_;

    unsigned int width_;
    unsigned int height_;

    boost::shared_ptr<IngameWindowRenderer> renderer_;
};

}
}

#endif
