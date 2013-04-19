/*
 * fluorescence is a free, customizable Ultima Online client.
 * Copyright (C) 2011-2012, http://fluorescence-client.org

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */



#include "worldview.hpp"

#include <misc/log.hpp>

#include <world/manager.hpp>
#include <world/sectormanager.hpp>
#include <world/lightmanager.hpp>
#include <world/map.hpp>
#include <world/statics.hpp>
#include <world/ingameobject.hpp>
#include <world/mobile.hpp>
#include <world/playerwalkmanager.hpp>

#include <ui/manager.hpp>
#include <ui/render/renderqueue.hpp>
#include <ui/cursormanager.hpp>
#include <ui/render/worldrenderer.hpp>
#include <ui/cliprectmanager.hpp>

#include <data/manager.hpp>
#include <data/artloader.hpp>

#include <algorithm>
#include <ClanLib/Display/Window/keys.h>

namespace fluo {
namespace ui {
namespace components {

WorldView::WorldView(CL_GUIComponent* parent) : GumpComponent(parent),
        centerTileX_(0), centerTileY_(0), centerTileZ_(0),
        lastCenterPixelX_(0), lastCenterPixelY_(0), zoom_(1) {
    renderer_.reset(new render::WorldRenderer(this));

    world::Manager::getSectorManager()->registerWorldView(this);
    setCenterObject(world::Manager::getSingleton()->getPlayer()->shared_from_this());

    set_constant_repaint(true);

    func_render().set(this, &WorldView::renderOneFrame);
    func_input_pressed().set(this, &WorldView::onInputPressed);
    func_input_released().set(this, &WorldView::onInputReleased);
    func_input_doubleclick().set(this, &WorldView::onDoubleClick);
    func_input_pointer_moved().set(this, &WorldView::onPointerMoved);
    func_pointer_exit().set(this, &WorldView::onPointerExit);
    func_pointer_enter().set(this, &WorldView::onPointerEnter);

    ui::Manager::getSingleton()->setWorldView(this);

    set_type_name("worldview");
}

WorldView::~WorldView() {
    world::Manager::getSectorManager()->unregisterWorldView(this);

    ui::Manager::getSingleton()->setWorldView(nullptr);
}

float WorldView::getCenterTileX() const {
    if (centerObject_) {
        return centerObject_->getLocXDraw();
    } else {
        return centerTileX_;
    }
}

float WorldView::getCenterTileY() const {
    if (centerObject_) {
        return centerObject_->getLocYDraw();
    } else {
        return centerTileY_;
    }
}

float WorldView::getCenterTileZ() const {
    if (centerObject_) {
        return centerObject_->getLocZDraw();
    } else {
        return centerTileZ_;
    }
}

void WorldView::setCenterTiles(float x, float y) {
    centerTileX_ = x;
    centerTileY_ = y;
}

float WorldView::getCenterPixelX() const {
    return floor(((getCenterTileX() - getCenterTileY()) * 22) + 0.5f);
}

float WorldView::getCenterPixelY() const {
    return floor(((getCenterTileX() + getCenterTileY()) * 22 - getCenterTileZ() * 4) + 0.5f);
}

CL_Vec2f WorldView::getTopLeftPixel() const {
    return CL_Vec2f(getCenterPixelX() - getDrawWidth()/2, getCenterPixelY() - getDrawHeight()/2);
}

unsigned int WorldView::getWidth() const {
    return get_width();
}

unsigned int WorldView::getHeight() const {
    return get_height();
}

float WorldView::getZoom() const {
    return zoom_;
}

void WorldView::zoomIn(float val) {
    zoom_ += val;
}

void WorldView::zoomOut(float val) {
    zoom_ -= val;
}

void WorldView::zoomReset() {
    zoom_ = 1.0;
}

unsigned int WorldView::getDrawWidth() const {
    return getWidth() / zoom_ + 0.5;
}

unsigned int WorldView::getDrawHeight() const {
    return getHeight() / zoom_ + 0.5;
}

CL_Size WorldView::getDrawSize() const {
    return CL_Size(getDrawWidth(), getDrawHeight());
}

void WorldView::renderOneFrame(CL_GraphicContext& gc, const CL_Rect& clipRect) {
    float pixelMoveX = getCenterPixelX() - lastCenterPixelX_;
    float pixelMoveY = getCenterPixelY() - lastCenterPixelY_;

    lastCenterPixelX_ = getCenterPixelX();
    lastCenterPixelY_ = getCenterPixelY();

    pixelMoveX *= zoom_;
    pixelMoveY *= zoom_;

    renderer_->moveCenter(pixelMoveX, pixelMoveY);

    CL_Draw::texture(gc, renderer_->getTexture(gc), CL_Rectf(0, 0, CL_Sizef(getWidth(), getHeight())));

    // particle effects are rendered on top, and not in the cached texture
    // because they are moving too fast, and it is hard to tell their bounding rectangle
    renderer_->renderParticleEffects(gc);
    renderer_->renderWeatherEffects(gc);
}

void WorldView::getRequiredSectors(std::list<IsoIndex>& list, unsigned int mapHeight, unsigned int cacheAdd) const {
    // at least, we need to load as much tiles as the diagonal of the view is long
    // we load this amount of tiles (plus a little cache) in each direction of the center tile

    double viewDiagonalPixel = sqrt(pow((float)getDrawWidth(), 2) + pow((float)getDrawHeight(), 2));
    double viewDiagonalTileCount = viewDiagonalPixel / 31;

    double viewDiagonalSectorCount = viewDiagonalTileCount / 8;
    int loadInEachDirection = (int)ceil(viewDiagonalSectorCount / 2);

    loadInEachDirection += cacheAdd; // some cache

    //LOG_DEBUG << "getRequiredSectors: diagonalPixel=" << viewDiagonalPixel << " diagonalTile=" << viewDiagonalTileCount <<
            //" diagonalSector=" << viewDiagonalSectorCount << " inEachDir=" << loadInEachDirection << std::endl;

    int centerSectorX = (int)(getCenterTileX() / 8.0);
    int centerSectorY = (int)(getCenterTileY() / 8.0);

    list.push_back(IsoIndex(centerSectorX, centerSectorY));

    // uncomment this to load just a single sector
    //return;

    unsigned int sectorX, sectorY;
    int diff;
    for (int x = -loadInEachDirection; x <= loadInEachDirection; ++x) {
        diff = loadInEachDirection - abs(x);
        for (int y = -diff; y <= diff; ++y) {
            sectorX = (std::max)(centerSectorX + x, 0);
            sectorY = (std::max)(centerSectorY + y, 0);
            list.push_back(IsoIndex(sectorX, sectorY));

        }
    }
}

bool WorldView::onInputPressed(const CL_InputEvent& e) {
    bool consumed = true;

    boost::shared_ptr<world::LightManager> lm;
    CL_Vec3f intensity;
    CL_Vec3f direction;
    boost::shared_ptr<world::IngameObject> clickedObject;

    //LOG_INFO << "input pressed WorldView: " << e.id << std::endl;

    switch (e.id) {
    case CL_MOUSE_RIGHT: {
        unsigned int direction = getDirectionForMousePosition(e.mouse_pos);
        world::Manager::getPlayerWalkManager()->setWalkDirection(direction);
        break;
    }

    //case CL_KEY_Q:
        //lm = world::Manager::getLightManager();
        //intensity = lm->getAmbientIntensity();
        //intensity.r += 0.1;
        //intensity.g += 0.1;
        //intensity.b += 0.1;
        //lm->setAmbientIntensity(intensity);
        //break;
    //case CL_KEY_W:
        //lm = world::Manager::getLightManager();
        //intensity = lm->getAmbientIntensity();
        //intensity.r = (std::max)(0.0, intensity.r - 0.1);
        //intensity.g = (std::max)(0.0, intensity.g - 0.1);
        //intensity.b = (std::max)(0.0, intensity.b - 0.1);
        //lm->setAmbientIntensity(intensity);
        //break;

    //case CL_KEY_A:
        //lm = world::Manager::getLightManager();
        //intensity = lm->getGlobalIntensity();
        //intensity.r += 0.5;
        //intensity.g += 0.5;
        //intensity.b += 0.5;
        //lm->setGlobalIntensity(intensity);
        //break;
    //case CL_KEY_S:
        //lm = world::Manager::getLightManager();
        //intensity = lm->getGlobalIntensity();
        //intensity.r = (std::max)(0.0, intensity.r - 0.2);
        //intensity.g = (std::max)(0.0, intensity.g - 0.2);
        //intensity.b = (std::max)(0.0, intensity.b - 0.2);
        //lm->setGlobalIntensity(intensity);
        //break;

    //case CL_KEY_R:
        //lm = world::Manager::getLightManager();
        //lm->setGlobalAngle(lm->getGlobalAngle() + 1);
        //LOG_DEBUG << "mw up " << lm->getGlobalAngle() << std::endl;
        //break;
    //case CL_KEY_E:
        //lm = world::Manager::getLightManager();
        //lm->setGlobalAngle(lm->getGlobalAngle() - 1);
        //LOG_DEBUG <<"mw down " << lm->getGlobalAngle() << std::endl;
        //break;

    //case CL_KEY_F:
        //data::Manager::getArtLoader()->printStats();
        //break;

    //case CL_KEY_N:
        //ui::Manager::getClipRectManager()->add(CL_Rectf(0, 0, CL_Sizef(800, 800)).translate(getTopLeftPixel()));
        //break;


    case CL_MOUSE_LEFT:
        clickedObject = getFirstIngameObjectAt(e.mouse_pos.x, e.mouse_pos.y);
        if (clickedObject && clickedObject->isDraggable()) {
            ui::Manager::getCursorManager()->setDragCandidate(clickedObject, e.mouse_pos.x, e.mouse_pos.y);
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
    case CL_MOUSE_RIGHT:
        world::Manager::getPlayerWalkManager()->stopAtNextTile();
        break;

    case CL_MOUSE_LEFT:
        clickedObject = getFirstIngameObjectAt(e.mouse_pos.x, e.mouse_pos.y);
        draggedObject = ui::Manager::getCursorManager()->stopDragging();

        if (clickedObject) {
            if (draggedObject) {
                ui::Manager::getSingleton()->onDragDrop(draggedObject, clickedObject);
            } else {
                ui::Manager::getSingleton()->onSingleClick(clickedObject);
            }
        } else if (draggedObject) {
            // dragged to void
            boost::shared_ptr<world::IngameObject> nullPtr;
            ui::Manager::getSingleton()->onDragDrop(draggedObject, nullPtr);
        } else {
            consumed = false;
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
            ui::Manager::getSingleton()->onDoubleClick(clickedObject);
        }

        return true;
    }

    return false;
}

boost::shared_ptr<world::IngameObject> WorldView::getFirstIngameObjectAt(unsigned int pixelX, unsigned int pixelY) const {
    //LOG_INFO << "WorldView::getFirstObjectAt " << pixelX << " " << pixelY << std::endl;
    float worldX = getCenterPixelX() - getDrawWidth()/2.0;
    worldX += (pixelX / zoom_);

    float worldY = getCenterPixelY() - getDrawHeight()/2.0;
    worldY += (pixelY / zoom_);

    return world::Manager::getSectorManager()->getFirstObjectAt(worldX, worldY, true);
}

void WorldView::setCenterObject(boost::shared_ptr<world::IngameObject> obj) {
    centerObject_ = obj;
}

bool WorldView::onPointerMoved(const CL_InputEvent& e) {
    unsigned int direction = getDirectionForMousePosition(e.mouse_pos);
    ui::Manager::getCursorManager()->setCursorDirection(direction);

    if (e.device.get_keycode(CL_MOUSE_RIGHT)) {
        world::Manager::getPlayerWalkManager()->setWalkDirection(direction);
        return true;
    }

    boost::shared_ptr<world::IngameObject> mouseOverObj = getFirstIngameObjectAt(e.mouse_pos.x, e.mouse_pos.y);
    ui::Manager::getSingleton()->setMouseOverObject(mouseOverObj);

    return false;
}

bool WorldView::onPointerExit() {
    ui::Manager::getCursorManager()->setCursorEnableFlags(ui::CursorEnableFlags::NONE);

    boost::shared_ptr<world::IngameObject> nullObj;
    ui::Manager::getSingleton()->setMouseOverObject(nullObj);

    return true;
}

bool WorldView::onPointerEnter() {
    ui::Manager::getCursorManager()->setCursorEnableFlags(ui::CursorEnableFlags::ALL);
    return true;
}

unsigned int WorldView::getDirectionForMousePosition(const CL_Point& mouse) const {
    float posY = mouse.y - (get_height() / 2);
    float posX = mouse.x - (get_width() / 2);
    float angle = atan2(posY, posX);

    unsigned int runDir = 0;
    if (fabs(posY) > 200 || fabs(posX) > 150) {
        runDir = Direction::RUNNING;
    }

    if (fabs(angle) >= 2.7489) {
        return runDir | Direction::SW;
    } else if (angle <= -1.9635) {
        return runDir | Direction::W;
    } else if (angle <= -1.1781) {
        return runDir | Direction::NW;
    } else if (angle <= -0.3927) {
        return runDir | Direction::N;
    } else if (angle <= 0.3927) {
        return runDir | Direction::NE;
    } else if (angle <= 1.1781) {
        return runDir | Direction::E;
    } else if (angle <= 1.9635) {
        return runDir | Direction::SE;
    } else {
        return runDir | Direction::S;
    }
}

CL_Mat4f WorldView::getViewMatrix() const {
    CL_Vec2f topLeft = getTopLeftPixel();
    CL_Mat4f ret = CL_Mat4f::scale(zoom_, zoom_, zoom_);

    ret.translate_self(-topLeft.x, -topLeft.y, 0);

    return ret;
}

}
}
}
