
#include "ingameview.hpp"

#include "ingameviewrenderer.hpp"

#include <misc/logger.hpp>

#include <world/manager.hpp>
#include <world/sectormanager.hpp>
#include <world/lightmanager.hpp>
#include <world/map.hpp>
#include <world/statics.hpp>

#include <ui/manager.hpp>
#include <ui/renderqueue.hpp>

#include <data/tiledataloader.hpp>

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
    double viewDiagonalTileCount = viewDiagonalPixel / 31;

    double viewDiagonalSectorCount = viewDiagonalTileCount / 8;
    int loadInEachDirection = (int)ceil(viewDiagonalSectorCount / 2);

    loadInEachDirection += cacheAdd; // some cache

    //LOGARG_DEBUG(LOGTYPE_UI, "getRequiredSectors: diagonalPixel=%lf diagonalTile=%lf diagonalSector=%lf inEachDir=%u", viewDiagonalPixel, viewDiagonalTileCount, viewDiagonalSectorCount, loadInEachDirection);

    int centerSectorX = (int)(getCenterTileX() / 8.0);
    int centerSectorY = (int)(getCenterTileY() / 8.0);

    list.push_back(centerSectorX * mapHeight + centerSectorY);
    //return;

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
    bool consumed = true;

    boost::shared_ptr<world::LightManager> lm;
    CL_Vec3f intensity;

    boost::shared_ptr<world::IngameObject> clickedItem;

    switch (e.id) {
    case CL_KEY_UP:
        centerTileY_ -= 1;
        break;
    case CL_KEY_DOWN:
        centerTileY_ += 1;
        break;
    case CL_KEY_LEFT:
        centerTileX_ -= 1;
        break;
    case CL_KEY_RIGHT:
        centerTileX_ += 1;
        break;
    case CL_KEY_R:
        lm = world::Manager::getSingleton()->getLightManager();
        intensity = lm->getAmbientIntensity();
        if (e.ctrl) {
            intensity.r = std::max(0.0, intensity.r - 0.1);
        } else {
            intensity.r += 0.1;
        }
        lm->setAmbientIntensity(intensity);
        break;
    case CL_KEY_G:
        lm = world::Manager::getSingleton()->getLightManager();
        intensity = lm->getAmbientIntensity();
        if (e.ctrl) {
            intensity.g = std::max(0.0, intensity.g - 0.1);
        } else {
            intensity.g += 0.1;
        }
        lm->setAmbientIntensity(intensity);
        break;
    case CL_KEY_B:
        lm = world::Manager::getSingleton()->getLightManager();
        intensity = lm->getAmbientIntensity();
        if (e.ctrl) {
            intensity.b = std::max(0.0, intensity.b - 0.1);
        } else {
            intensity.b += 0.1;
        }
        lm->setAmbientIntensity(intensity);
        break;
    case CL_KEY_ADD:
        lm = world::Manager::getSingleton()->getLightManager();
        intensity = lm->getAmbientIntensity();
        intensity.r += 0.1;
        intensity.g += 0.1;
        intensity.b += 0.1;
        lm->setAmbientIntensity(intensity);
        break;
    case CL_KEY_SUBTRACT:
        lm = world::Manager::getSingleton()->getLightManager();
        intensity = lm->getAmbientIntensity();
        intensity.r = std::max(0.0, intensity.r - 0.1);
        intensity.g = std::max(0.0, intensity.g - 0.1);
        intensity.b = std::max(0.0, intensity.b - 0.1);
        lm->setAmbientIntensity(intensity);
        break;

    case CL_MOUSE_WHEEL_UP:
        lm = world::Manager::getSingleton()->getLightManager();
        lm->setGlobalAngle(lm->getGlobalAngle() + 5);
        LOGARG_DEBUG(LOGTYPE_UI, "mw up %f", lm->getGlobalAngle());
        break;
    case CL_MOUSE_WHEEL_DOWN:
        lm = world::Manager::getSingleton()->getLightManager();
        lm->setGlobalAngle(lm->getGlobalAngle() - 5);
        LOGARG_DEBUG(LOGTYPE_UI, "mw down %f", lm->getGlobalAngle());
        break;

    case CL_MOUSE_LEFT:
        clickedItem = getFirstIngameObjectAt(e.mouse_pos.x, e.mouse_pos.y);
        if (!clickedItem) {
            LOG_DEBUG(LOGTYPE_UI, "Clicked, but found no item");
        }
        break;

    default:
        consumed = false;
        break;
    }

    if (clickedItem) {
        world::MapTile* tile = dynamic_cast<world::MapTile*>(clickedItem.get());
        if (tile) {
            LOGARG_INFO(LOGTYPE_UI, "Clicked map, id=%x loc=(%i/%i/%i) name=%S",
                tile->getArtId(), tile->getLocX(), tile->getLocY(), tile->getLocZ(), const_cast<data::LandTileInfo*>(tile->getTileDataInfo())->name_.getTerminatedBuffer());
        }

        world::StaticItem* itm = dynamic_cast<world::StaticItem*>(clickedItem.get());
        if (itm) {
            LOGARG_INFO(LOGTYPE_UI, "Clicked static, id=%x loc=(%i/%i/%i) name=%S",
                itm->getArtId(), itm->getLocX(), itm->getLocY(), itm->getLocZ(), const_cast<data::StaticTileInfo*>(itm->getTileDataInfo())->name_.getTerminatedBuffer());
        }

        LOGARG_INFO(LOGTYPE_UI, "Vertex normals: %.2f/%.2f/%2.f %.2f/%.2f/%2.f %.2f/%.2f/%2.f %.2f/%.2f/%2.f %.2f/%.2f/%2.f %.2f/%.2f/%2.f ",
            clickedItem->getVertexNormals()[0].x, clickedItem->getVertexNormals()[0].y, clickedItem->getVertexNormals()[0].z,
            clickedItem->getVertexNormals()[1].x, clickedItem->getVertexNormals()[1].y, clickedItem->getVertexNormals()[0].z,
            clickedItem->getVertexNormals()[2].x, clickedItem->getVertexNormals()[2].y, clickedItem->getVertexNormals()[0].z,
            clickedItem->getVertexNormals()[3].x, clickedItem->getVertexNormals()[3].y, clickedItem->getVertexNormals()[0].z,
            clickedItem->getVertexNormals()[4].x, clickedItem->getVertexNormals()[4].y, clickedItem->getVertexNormals()[0].z,
            clickedItem->getVertexNormals()[5].x, clickedItem->getVertexNormals()[5].y, clickedItem->getVertexNormals()[0].z);


        if (e.shift) {
            clickedItem->setVisible(false);
        }
    }

    return consumed;
}

boost::shared_ptr<world::IngameObject> IngameView::getFirstIngameObjectAt(unsigned int pixelX, unsigned int pixelY) {
    LOGARG_INFO(LOGTYPE_UI, "IngameView::getFirstObjectAt %u %u", pixelX, pixelY);
    int worldX = getCenterPixelX() - get_width()/2.0;
    worldX += pixelX;

    int worldY = getCenterPixelY() - get_height()/2.0;
    worldY += pixelY;

    return ui::Manager::getSingleton()->getRenderQueue()->getFirstIngameObjectAt(worldX, worldY);
}

}
}
