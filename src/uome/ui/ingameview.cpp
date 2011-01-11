
#include "ingameview.hpp"

#include "ingameviewrenderer.hpp"

#include <misc/logger.hpp>

#include <world/manager.hpp>
#include <world/sectormanager.hpp>

#include <ClanLib/Display/Window/keys.h>

namespace uome {
namespace ui {

IngameView::IngameView(const CL_Rect& bounds) : GumpMenu(bounds),
        centerTileX_(0), centerTileY_(0) {
    renderer_.reset(new IngameViewRenderer(this));

    world::Manager::getSingleton()->getSectorManager()->registerIngameView(this);

    set_constant_repaint(true);

    func_render().set(this, &IngameView::renderOneFrame);
    func_input_pressed().set(this, &IngameView::onInputPressed);
}

IngameView::~IngameView() {
    LOG_DEBUG(LOGTYPE_UI, "IngameView dtor");
    world::Manager::getSingleton()->getSectorManager()->unregisterIngameView(this);
}

unsigned int IngameView::getCenterTileX() {
    return centerTileX_;
}

unsigned int IngameView::getCenterTileY() {
    return centerTileY_;
}

void IngameView::setCenterTiles(unsigned int x, unsigned int y) {
    centerTileX_ = x;
    centerTileY_ = y;
}

int IngameView::getCenterPixelX() {
    return (getCenterTileX() - getCenterTileY()) * 22;
}

int IngameView::getCenterPixelY() {
    return (getCenterTileX() + getCenterTileY()) * 22;;
}

unsigned int IngameView::getWidth() {
    return get_width();
}

unsigned int IngameView::getHeight() {
    return get_height();
}

void IngameView::renderOneFrame(CL_GraphicContext& gc, const CL_Rect& clipRect) {
    renderer_->renderOneFrame(gc, clipRect);
}

void IngameView::getRequiredSectors(std::list<unsigned int>& list, unsigned int mapHeight, unsigned int cacheAdd) {
    // at least, we need to load as much tiles as the diagonal of the view is long
    // we load this amount of tiles (plus a little cache) in each direction of the center tile

    double viewDiagonalPixel = sqrt(pow(getWidth(), 2) + pow(getHeight(), 2));
    double viewDiagonalTileCount = viewDiagonalPixel / 44;

    double viewDiagonalSectorCount = viewDiagonalTileCount / 8;
    int loadInEachDirection = (int)ceil(viewDiagonalSectorCount / 2);

    loadInEachDirection += cacheAdd; // some cache

    //LOGARG_DEBUG(LOGTYPE_UI, "getRequiredSectors: diagonalPixel=%lf diagonalTile=%lf diagonalSector=%lf inEachDir=%u", viewDiagonalPixel, viewDiagonalTileCount, viewDiagonalSectorCount, loadInEachDirection);

    int centerSectorX = (int)(getCenterTileX() / 8.0);
    int centerSectorY = (int)(getCenterTileY() / 8.0);

    unsigned int sectorX, sectorY, sectorId;
    int diff;
    for (int x = -loadInEachDirection; x <= loadInEachDirection; ++x) {
        diff = loadInEachDirection - abs(x);
        for (int y = -diff; y <= diff; ++y) {
            sectorX = std::max(centerSectorX + x, 0);
            sectorY = std::max(centerSectorY + y, 0);
            sectorId = sectorX * mapHeight + sectorY;
            list.push_back(sectorId);

        }
    }
}

bool IngameView::onInputPressed(const CL_InputEvent& e) {
    bool consumed = false;

    if (e.id == CL_KEY_UP) {
        centerTileY_ -= 1;
        consumed = true;
    } else if (e.id == CL_KEY_DOWN) {
        centerTileY_ += 1;
        consumed = true;
    } else if (e.id == CL_KEY_LEFT) {
        centerTileX_ -= 1;
        consumed = true;
    } else if (e.id == CL_KEY_RIGHT) {
        centerTileX_ += 1;
        consumed = true;
    }

    return consumed;
}

}
}
