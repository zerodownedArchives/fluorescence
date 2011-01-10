
#include "ingamewindow.hpp"

#include "ingamewindowrenderer.hpp"

namespace uome {
namespace ui {

IngameWindow::IngameWindow(CL_GUIManager* manager) : UiObject(manager),
        centerTileX_(0), centerTileY_(0) {
    renderer_.reset(new IngameWindowRenderer(this));

    set_constant_repaint(true);

    func_render().set(this, &IngameWindow::renderOneFrame);
}

unsigned int IngameWindow::getCenterTileX() {
    return centerTileX_;
}

unsigned int IngameWindow::getCenterTileY() {
    return centerTileY_;
}

void IngameWindow::setCenterTiles(unsigned int x, unsigned int y) {
    centerTileX_ = x;
    centerTileY_ = y;
}

int IngameWindow::getCenterPixelX() {
    return (getCenterTileX() - getCenterTileY()) * 22;
}

int IngameWindow::getCenterPixelY() {
    return (getCenterTileX() + getCenterTileY()) * 22;;
}

unsigned int IngameWindow::getWidth() {
    return get_width();
}

unsigned int IngameWindow::getHeight() {
    return get_height();
}

void IngameWindow::renderOneFrame(CL_GraphicContext& gc, const CL_Rect& clipRect) {
    renderer_->renderOneFrame(gc, clipRect);
}

}

}
