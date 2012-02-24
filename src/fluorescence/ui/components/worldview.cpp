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
#include <ui/doubleclickhandler.hpp>
#include <ui/cursormanager.hpp>
#include <ui/render/worldviewrenderer.hpp>
#include <ui/cliprectmanager.hpp>

#include <data/manager.hpp>
#include <data/artloader.hpp>

#include <algorithm>
#include <ClanLib/Display/Window/keys.h>

namespace fluo {
namespace ui {
namespace components {

WorldView::WorldView(CL_GUIComponent* parent, const CL_Rect& bounds) : GumpElement(parent),
        centerTileX_(0), centerTileY_(0), centerTileZ_(0),
        lastCenterPixelX_(0), lastCenterPixelY_(0) {
    this->set_geometry(bounds);
    renderer_.reset(new render::WorldViewRenderer(this));

    world::Manager::getSectorManager()->registerWorldView(this);
    setCenterObject(world::Manager::getSingleton()->getPlayer()->shared_from_this());

    set_constant_repaint(true);

    func_render().set(this, &WorldView::renderOneFrame);
    func_input_pressed().set(this, &WorldView::onInputPressed);
    func_input_released().set(this, &WorldView::onInputReleased);
    func_input_doubleclick().set(this, &WorldView::onDoubleClick);
    func_input_pointer_moved().set(this, &WorldView::onPointerMoved);
}

WorldView::~WorldView() {
    world::Manager::getSectorManager()->unregisterWorldView(this);
}

float WorldView::getCenterTileX() const {
    if (centerObject_) {
        return centerObject_->getLocX();
    } else {
        return centerTileX_;
    }
}

float WorldView::getCenterTileY() const {
    if (centerObject_) {
        return centerObject_->getLocY();
    } else {
        return centerTileY_;
    }
}

float WorldView::getCenterTileZ() const {
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

float WorldView::getCenterPixelX() const {
    return floor(((getCenterTileX() - getCenterTileY()) * 22) + 0.5f);
}

float WorldView::getCenterPixelY() const {
    return floor(((getCenterTileX() + getCenterTileY()) * 22 - getCenterTileZ() * 4) + 0.5f);
}

CL_Vec2f WorldView::getTopLeftPixel() const {
    return CL_Vec2f(getCenterPixelX() - getWidth()/2, getCenterPixelY() - getHeight()/2);
}

unsigned int WorldView::getWidth() const {
    return get_width();
}

unsigned int WorldView::getHeight() const {
    return get_height();
}

void WorldView::renderOneFrame(CL_GraphicContext& gc, const CL_Rect& clipRect) {
    float pixelMoveX = getCenterPixelX() - lastCenterPixelX_;
    float pixelMoveY = getCenterPixelY() - lastCenterPixelY_;
    
    lastCenterPixelX_ = getCenterPixelX();
    lastCenterPixelY_ = getCenterPixelY();
    
    renderer_->moveCenter(pixelMoveX, pixelMoveY);
    
    CL_Draw::texture(gc, *renderer_->getTexture(gc)->getTexture(), CL_Rectf(0, 0, CL_Sizef(getWidth(), getHeight())));
}

void WorldView::getRequiredSectors(std::list<IsoIndex>& list, unsigned int mapHeight, unsigned int cacheAdd) {
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

    //LOGARG_INFO(LOGTYPE_INPUT, "input pressed WorldView: %u", e.id);

    switch (e.id) {
    case CL_MOUSE_RIGHT: {
        //CL_Vec2f yaxis(0, 1);
        //CL_Vec2f mouseVec(e.mouse_pos.x, e.mouse_pos.y)
        //angle = arccos
        unsigned int direction = getDirectionForMousePosition(e.mouse_pos);
        world::Manager::getPlayerWalkManager()->setWalkDirection(direction);
        
        //world::Manager::getPlayerWalkManager()->setWalkDirection(Direction::N);
        break;
    }

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

    case CL_KEY_R:
        lm = world::Manager::getLightManager();
        lm->setGlobalAngle(lm->getGlobalAngle() + 1);
        LOG_DEBUG << "mw up " << lm->getGlobalAngle() << std::endl;
        break;
    case CL_KEY_E:
        lm = world::Manager::getLightManager();
        lm->setGlobalAngle(lm->getGlobalAngle() - 1);
        LOG_DEBUG <<"mw down " << lm->getGlobalAngle() << std::endl;
        break;

    case CL_KEY_F:
        data::Manager::getArtLoader()->printStats();
        break;
        
    case CL_KEY_N:
        ui::Manager::getClipRectManager()->add(CL_Rectf(0, 0, CL_Sizef(800, 800)).translate(getTopLeftPixel()));
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
    case CL_MOUSE_RIGHT:
        world::Manager::getPlayerWalkManager()->stopAtNextTile();
        break;
        
    case CL_MOUSE_LEFT:
        clickedObject = getFirstIngameObjectAt(e.mouse_pos.x, e.mouse_pos.y);
        draggedObject = ui::Manager::getSingleton()->getCursorManager()->stopDragging();

        if (clickedObject) {
            if (draggedObject) {
                ui::Manager::getSingleton()->queueDrag(draggedObject, clickedObject);
            } else {
                ui::Manager::getSingleton()->onClickEvent(clickedObject);
            }
        } else if (draggedObject) {
            // dragged to void
            boost::shared_ptr<world::IngameObject> nullPtr;
            ui::Manager::getSingleton()->queueDrag(draggedObject, nullPtr);
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
            clickedObject->onClick();
        }

        return true;
    }

    return false;
}

boost::shared_ptr<world::IngameObject> WorldView::getFirstIngameObjectAt(unsigned int pixelX, unsigned int pixelY) {
    //LOG_INFO << "WorldView::getFirstObjectAt " << pixelX << " " << pixelY << std::endl;
    float worldX = getCenterPixelX() - get_width()/2.0;
    worldX += pixelX;

    float worldY = getCenterPixelY() - get_height()/2.0;
    worldY += pixelY;

    return world::Manager::getSectorManager()->getFirstObjectAt(worldX, worldY, true);
    boost::shared_ptr<world::IngameObject> ret;
    return ret;
}

void WorldView::setCenterObject(boost::shared_ptr<world::IngameObject> obj) {
    centerObject_ = obj;
}

bool WorldView::onPointerMoved(const CL_InputEvent& e) {
    if (e.device.get_keycode(CL_MOUSE_RIGHT)) {
        unsigned int direction = getDirectionForMousePosition(e.mouse_pos);
        world::Manager::getPlayerWalkManager()->setWalkDirection(direction);
    }
    
    return true;
}

unsigned int WorldView::getDirectionForMousePosition(const CL_Point& mouse) const {
    float posY = mouse.y - (get_height() / 2);
    float posX = mouse.x - (get_width() / 2);
    float angle = atan2(posY, posX);
    
    float absangle = fabs(angle);
    if (absangle >= 2.7489) {
        return Direction::SW;
    } else if (angle <= -1.9635) {
        return Direction::W;
    } else if (angle <= -1.1781) {
        return Direction::NW;
    } else if (angle <= -0.3927) {
        return Direction::N;
    } else if (angle <= 0.3927) {
        return Direction::NE;
    } else if (angle <= 1.1781) {
        return Direction::E;
    } else if (angle <= 1.9635) {
        return Direction::SE;
    } else {
        return Direction::S;
    }
}

}
}
}
