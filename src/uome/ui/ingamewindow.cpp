
#include "ingamewindow.hpp"

#include "ingamewindowrenderer.hpp"

namespace uome {
namespace ui {

IngameWindow::IngameWindow(unsigned int width, unsigned int height) :
        centerTileX_(0), centerTileY_(0),
        width_(width), height_(height) {
    renderer_.reset(new IngameWindowRenderer(this));
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
    return width_;
}

unsigned int IngameWindow::getHeight() {
    return height_;
}

void IngameWindow::setWidth(unsigned int width) {
    width_ = width;
}

void IngameWindow::setHeight(unsigned int height) {
    height_ = height;
}

void IngameWindow::renderOneFrame() {
    renderer_->renderOneFrame();
}

}

}
