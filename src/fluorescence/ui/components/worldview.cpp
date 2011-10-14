
#include "worldview.hpp"

#include <misc/log.hpp>

#include <world/manager.hpp>
#include <world/sectormanager.hpp>
#include <world/lightmanager.hpp>
#include <world/map.hpp>
#include <world/statics.hpp>
#include <world/ingameobject.hpp>

#include <ui/manager.hpp>
#include <ui/renderqueue.hpp>
#include <ui/doubleclickhandler.hpp>
#include <ui/cursormanager.hpp>
#include <ui/render/worldviewrenderer.hpp>

#include <data/manager.hpp>
#include <data/artloader.hpp>

#include <algorithm>
#include <ClanLib/Display/Window/keys.h>

namespace fluo {
namespace ui {

WorldView::WorldView(CL_GUIComponent* parent, const CL_Rect& bounds) : GumpElement(parent),
        centerTileX_(0), centerTileY_(0), centerTileZ_(0) {
    this->set_geometry(bounds);
    renderer_.reset(new WorldViewRenderer(ui::Manager::getWorldRenderQueue(), this));

    world::Manager::getSectorManager()->registerWorldView(this);
    setCenterObject(world::Manager::getSingleton()->getPlayer());

    set_constant_repaint(true);

    func_render().set(this, &WorldView::renderOneFrame);
    func_input_pressed().set(this, &WorldView::onInputPressed);
    func_input_released().set(this, &WorldView::onInputReleased);
    func_input_doubleclick().set(this, &WorldView::onDoubleClick);
}

WorldView::~WorldView() {
    world::Manager::getSectorManager()->unregisterWorldView(this);
}

float WorldView::getCenterTileX() {
    if (centerObject_) {
        return centerObject_->getLocX();
    } else {
        return centerTileX_;
    }
}

float WorldView::getCenterTileY() {
    if (centerObject_) {
        return centerObject_->getLocY();
    } else {
        return centerTileY_;
    }
}

float WorldView::getCenterTileZ() {
    if (centerObject_) {
        return centerObject_->getLocZ();
    } else {
        return centerTileZ_;
    }
}

void WorldView::setCenterTiles(float x, float y) {
    centerTileX_ = x;
    centerTileY_ = y;
}

int WorldView::getCenterPixelX() {
    return (getCenterTileX() - getCenterTileY()) * 22;
}

int WorldView::getCenterPixelY() {
    return (getCenterTileX() + getCenterTileY()) * 22 - getCenterTileZ() * 4;
}

unsigned int WorldView::getWidth() {
    return get_width();
}

unsigned int WorldView::getHeight() {
    return get_height();
}

void WorldView::renderOneFrame(CL_GraphicContext& gc, const CL_Rect& clipRect) {
    gc.push_cliprect(get_geometry());
    //CL_Draw::texture(gc, *renderer_->getTexture(gc)->getTexture(), CL_Rectf(0, 0, CL_Sizef(getWidth(), getHeight())));
    renderer_->render(gc);
    gc.pop_cliprect();
}

void WorldView::getRequiredSectors(std::list<unsigned int>& list, unsigned int mapHeight, unsigned int cacheAdd) {
    // at least, we need to load as much tiles as the diagonal of the view is long
    // we load this amount of tiles (plus a little cache) in each direction of the center tile

    double viewDiagonalPixel = sqrt(pow((float)getWidth(), 2) + pow((float)getHeight(), 2));
    double viewDiagonalTileCount = viewDiagonalPixel / 31;

    double viewDiagonalSectorCount = viewDiagonalTileCount / 8;
    int loadInEachDirection = (int)ceil(viewDiagonalSectorCount / 2);

    loadInEachDirection += cacheAdd; // some cache

    //LOG_DEBUG << "getRequiredSectors: diagonalPixel=" << viewDiagonalPixel << " diagonalTile=" << viewDiagonalTileCount <<
            //" diagonalSector=" << viewDiagonalSectorCount << " inEachDir=" << loadInEachDirection << std::endl;

    int centerSectorX = (int)(getCenterTileX() / 8.0);
    int centerSectorY = (int)(getCenterTileY() / 8.0);

    list.push_back(centerSectorX * mapHeight + centerSectorY);

    // uncomment this to load just a single sector
    //return;

    unsigned int sectorX, sectorY, sectorId;
    int diff;
    for (int x = -loadInEachDirection; x <= loadInEachDirection; ++x) {
        diff = loadInEachDirection - abs(x);
        for (int y = -diff; y <= diff; ++y) {
            sectorX = (std::max)(centerSectorX + x, 0);
            sectorY = (std::max)(centerSectorY + y, 0);
            sectorId = sectorX * mapHeight + sectorY;
            list.push_back(sectorId);

        }
    }
}

bool WorldView::onInputPressed(const CL_InputEvent& e) {
    bool consumed = true;

    boost::shared_ptr<world::LightManager> lm;
    CL_Vec3f intensity;
    CL_Vec3f direction;
    boost::shared_ptr<world::IngameObject> clickedObject;

    //LOGARG_INFO(LOGTYPE_INPUT, "input pressed WorldView: %u", e.id);

    switch (e.id) {
    case CL_KEY_UP:
        centerTileY_ -= .1;
        break;
    case CL_KEY_DOWN:
        centerTileY_ += .1;
        break;
    case CL_KEY_LEFT:
        centerTileX_ -= .1;
        break;
    case CL_KEY_RIGHT:
        centerTileX_ += .1;
        break;

    case CL_KEY_ADD:
        lm = world::Manager::getLightManager();
        intensity = lm->getAmbientIntensity();
        intensity.r += 0.1;
        intensity.g += 0.1;
        intensity.b += 0.1;
        lm->setAmbientIntensity(intensity);
        break;
    case CL_KEY_SUBTRACT:
        lm = world::Manager::getLightManager();
        intensity = lm->getAmbientIntensity();
        intensity.r = (std::max)(0.0, intensity.r - 0.1);
        intensity.g = (std::max)(0.0, intensity.g - 0.1);
        intensity.b = (std::max)(0.0, intensity.b - 0.1);
        lm->setAmbientIntensity(intensity);
        break;

    case CL_KEY_A:
        lm = world::Manager::getLightManager();
        intensity = lm->getGlobalIntensity();
        intensity.r += 0.03;
        intensity.g += 0.03;
        intensity.b += 0.03;
        lm->setGlobalIntensity(intensity);
        break;
    case CL_KEY_S:
        lm = world::Manager::getLightManager();
        intensity = lm->getGlobalIntensity();
        intensity.r = (std::max)(0.0, intensity.r - 0.03);
        intensity.g = (std::max)(0.0, intensity.g - 0.03);
        intensity.b = (std::max)(0.0, intensity.b - 0.03);
        lm->setGlobalIntensity(intensity);
        break;

    case CL_MOUSE_WHEEL_UP:
        lm = world::Manager::getLightManager();
        lm->setGlobalAngle(lm->getGlobalAngle() + 5);
        LOG_DEBUG << "mw up " << lm->getGlobalAngle() << std::endl;
        break;
    case CL_MOUSE_WHEEL_DOWN:
        lm = world::Manager::getLightManager();
        lm->setGlobalAngle(lm->getGlobalAngle() - 5);
        LOG_DEBUG <<"mw down " << lm->getGlobalAngle() << std::endl;
        break;

    case CL_KEY_F:
        data::Manager::getArtLoader()->printStats();
        LOG_DEBUG << "Render queue count: " << ui::Manager::getWorldRenderQueue()->size() << std::endl;
        break;


    case CL_MOUSE_LEFT:
        set_focus();
        clickedObject = getFirstIngameObjectAt(e.mouse_pos.x, e.mouse_pos.y);
        if (clickedObject && clickedObject->isDraggable()) {
            ui::Manager::getSingleton()->getCursorManager()->setDragCandidate(clickedObject, e.mouse_pos.x, e.mouse_pos.y);
        }
        break;

    default:
        consumed = false;
        break;
    }

    return consumed;
}

bool WorldView::onInputReleased(const CL_InputEvent& e) {
    bool consumed = true;

    boost::shared_ptr<world::IngameObject> clickedObject;
    boost::shared_ptr<world::IngameObject> draggedObject;

    switch (e.id) {
    case CL_MOUSE_LEFT:
        clickedObject = getFirstIngameObjectAt(e.mouse_pos.x, e.mouse_pos.y);
        draggedObject = ui::Manager::getSingleton()->getCursorManager()->stopDragging();

        if (clickedObject) {
            if (draggedObject) {
                draggedObject->onDraggedOnto(clickedObject);
            } else {
                ui::Manager::getDoubleClickHandler()->notify(clickedObject);
            }
        } else if (draggedObject) {
            // dragged to void
            draggedObject->onDraggedToVoid();
        }
        break;

    default:
        consumed = false;
        break;
    }

    return consumed;
}

bool WorldView::onDoubleClick(const CL_InputEvent& e) {
    if (e.id == CL_MOUSE_LEFT) {
        boost::shared_ptr<world::IngameObject> clickedObject = getFirstIngameObjectAt(e.mouse_pos.x, e.mouse_pos.y);
        if (!clickedObject) {
            LOG_DEBUG << "doublelicked, but found no object" << std::endl;
        } else {
            clickedObject->onClick();
        }

        return true;
    }

    return false;
}

boost::shared_ptr<world::IngameObject> WorldView::getFirstIngameObjectAt(unsigned int pixelX, unsigned int pixelY) {
    //LOG_INFO << "WorldView::getFirstObjectAt " << pixelX << " " << pixelY << std::endl;
    int worldX = getCenterPixelX() - get_width()/2.0;
    worldX += pixelX;

    int worldY = getCenterPixelY() - get_height()/2.0;
    worldY += pixelY;

    return ui::Manager::getWorldRenderQueue()->getFirstObjectAt(worldX, worldY, true);
}

void WorldView::setCenterObject(boost::shared_ptr<world::IngameObject> obj) {
    centerObject_ = obj;
}

}
}
