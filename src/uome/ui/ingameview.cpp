
#include "ingameview.hpp"

#include "ingameviewrenderer.hpp"

#include <misc/log.hpp>

#include <world/manager.hpp>
#include <world/sectormanager.hpp>
#include <world/lightmanager.hpp>
#include <world/map.hpp>
#include <world/statics.hpp>

#include <ui/manager.hpp>
#include <ui/renderqueue.hpp>
#include <ui/doubleclickhandler.hpp>

#include <data/manager.hpp>
#include <data/artloader.hpp>

#include <ClanLib/Display/Window/keys.h>

namespace uome {
namespace ui {

IngameView::IngameView(CL_GUIComponent* parent, const CL_Rect& bounds) : GumpElement(parent),
        centerTileX_(0), centerTileY_(0) {
    this->set_geometry(bounds);
    renderer_.reset(new IngameViewRenderer(this));

    world::Manager::getSingleton()->getSectorManager()->registerIngameView(this);

    set_constant_repaint(true);

    func_render().set(this, &IngameView::renderOneFrame);
    func_input_pressed().set(this, &IngameView::onInputPressed);
    func_input_released().set(this, &IngameView::onInputReleased);
    func_input_doubleclick().set(this, &IngameView::onDoubleClick);
}

IngameView::~IngameView() {
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
    gc.push_cliprect(get_geometry());
    renderer_->renderOneFrame(gc, clipRect);
    gc.pop_cliprect();
}

void IngameView::getRequiredSectors(std::list<unsigned int>& list, unsigned int mapHeight, unsigned int cacheAdd) {
    // at least, we need to load as much tiles as the diagonal of the view is long
    // we load this amount of tiles (plus a little cache) in each direction of the center tile

    double viewDiagonalPixel = sqrt(pow(getWidth(), 2) + pow(getHeight(), 2));
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
    CL_Vec3f direction;

    //LOGARG_INFO(LOGTYPE_INPUT, "input pressed ingameview: %u", e.id);

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

    case CL_KEY_A:
        lm = world::Manager::getSingleton()->getLightManager();
        intensity = lm->getGlobalIntensity();
        intensity.r += 0.1;
        intensity.g += 0.1;
        intensity.b += 0.1;
        lm->setGlobalIntensity(intensity);
        break;
    case CL_KEY_S:
        lm = world::Manager::getSingleton()->getLightManager();
        intensity = lm->getGlobalIntensity();
        intensity.r = std::max(0.0, intensity.r - 0.1);
        intensity.g = std::max(0.0, intensity.g - 0.1);
        intensity.b = std::max(0.0, intensity.b - 0.1);
        lm->setGlobalIntensity(intensity);
        break;

    case CL_MOUSE_WHEEL_UP:
        lm = world::Manager::getSingleton()->getLightManager();
        lm->setGlobalAngle(lm->getGlobalAngle() + 5);
        LOG_DEBUG << "mw up " << lm->getGlobalAngle() << std::endl;
        break;
    case CL_MOUSE_WHEEL_DOWN:
        lm = world::Manager::getSingleton()->getLightManager();
        lm->setGlobalAngle(lm->getGlobalAngle() - 5);
        LOG_DEBUG <<"mw down " << lm->getGlobalAngle() << std::endl;
        break;

    case CL_MOUSE_LEFT:
        set_focus();
        break;


    case CL_KEY_F:
        data::Manager::getArtLoader()->printStats();
        break;

    default:consumed = false;
    }

    return consumed;
}

bool IngameView::onInputReleased(const CL_InputEvent& e) {
    bool consumed = true;

    boost::shared_ptr<world::IngameObject> clickedObject;

    switch (e.id) {
    case CL_MOUSE_LEFT:
        clickedObject = getFirstIngameObjectAt(e.mouse_pos.x, e.mouse_pos.y);
        if (!clickedObject) {
            LOG_DEBUG << "Clicked, but found no object" << std::endl;
        } else {
            ui::Manager::getDoubleClickHandler()->notify(clickedObject);
        }
        break;

    default:
        consumed = false;
        break;
    }

    return consumed;
}

bool IngameView::onDoubleClick(const CL_InputEvent& e) {
    //if (e.id == CL_MOUSE_LEFT) {
        boost::shared_ptr<world::IngameObject> clickedObject = getFirstIngameObjectAt(e.mouse_pos.x, e.mouse_pos.y);
        if (!clickedObject) {
            LOG_DEBUG << "doublelicked, but found no object" << std::endl;
        } else {
            clickedObject->onClick();
        }

        return true;
    //}

    return false;
}

boost::shared_ptr<world::IngameObject> IngameView::getFirstIngameObjectAt(unsigned int pixelX, unsigned int pixelY) {
    LOG_INFO << "IngameView::getFirstObjectAt " << pixelX << " " << pixelY << std::endl;
    int worldX = getCenterPixelX() - get_width()/2.0;
    worldX += pixelX;

    int worldY = getCenterPixelY() - get_height()/2.0;
    worldY += pixelY;

    return ui::Manager::getSingleton()->getRenderQueue()->getFirstIngameObjectAt(worldX, worldY);
}

}
}
