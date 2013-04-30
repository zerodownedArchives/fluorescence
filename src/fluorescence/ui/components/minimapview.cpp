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



#include "minimapview.hpp"

#include <misc/log.hpp>

#include <world/manager.hpp>
#include <world/sectormanager.hpp>
#include <world/map.hpp>
#include <world/statics.hpp>
#include <world/ingameobject.hpp>
#include <world/mobile.hpp>

#include <ui/manager.hpp>
#include <ui/render/renderqueue.hpp>
#include <ui/cursormanager.hpp>
#include <ui/render/minimaprenderer.hpp>

#include <data/manager.hpp>
#include <data/artloader.hpp>


namespace fluo {
namespace ui {
namespace components {

MiniMapView::MiniMapView(CL_GUIComponent* parent) : GumpComponent(parent), SectorView(false),
        centerTileX_(0), centerTileY_(0), centerTileZ_(0), zoom_(1) {
    renderer_.reset(new render::MiniMapRenderer(this));

    setCenterObject(world::Manager::getSingleton()->getPlayer()->shared_from_this());

    func_render().set(this, &MiniMapView::renderOneFrame);
    //func_input_pressed().set(this, &MiniMapView::onInputPressed);
    //func_input_released().set(this, &MiniMapView::onInputReleased);
    //func_input_doubleclick().set(this, &MiniMapView::onDoubleClick);
    //func_input_pointer_moved().set(this, &MiniMapView::onPointerMoved);
    func_pointer_exit().set(this, &MiniMapView::onPointerExit);
    func_pointer_enter().set(this, &MiniMapView::onPointerEnter);

    set_type_name("minimapview");
}

float MiniMapView::getCenterTileX() const {
    if (centerObject_) {
        return centerObject_->getLocXDraw();
    } else {
        return centerTileX_;
    }
}

float MiniMapView::getCenterTileY() const {
    if (centerObject_) {
        return centerObject_->getLocYDraw();
    } else {
        return centerTileY_;
    }
}

float MiniMapView::getCenterTileZ() const {
    if (centerObject_) {
        return centerObject_->getLocZDraw();
    } else {
        return centerTileZ_;
    }
}

void MiniMapView::setCenterTiles(float x, float y) {
    centerTileX_ = x;
    centerTileY_ = y;
}

float MiniMapView::getCenterPixelX() const {
    return getCenterTileX();
}

float MiniMapView::getCenterPixelY() const {
    return getCenterTileY();
}

CL_Vec2f MiniMapView::getTopLeftPixel() const {
    return CL_Vec2f(getCenterPixelX() - getDrawWidth()/2, getCenterPixelY() - getDrawHeight()/2);
}

unsigned int MiniMapView::getWidth() const {
    return get_width();
}

unsigned int MiniMapView::getHeight() const {
    return get_height();
}

float MiniMapView::getZoom() const {
    return zoom_;
}

void MiniMapView::zoomIn(float val) {
    zoom_ += val;
}

void MiniMapView::zoomOut(float val) {
    zoom_ -= val;
}

void MiniMapView::zoomReset() {
    zoom_ = 1.0;
}

unsigned int MiniMapView::getDrawWidth() const {
    return getWidth() / zoom_ + 0.5;
}

unsigned int MiniMapView::getDrawHeight() const {
    return getHeight() / zoom_ + 0.5;
}

CL_Size MiniMapView::getDrawSize() const {
    return CL_Size(getDrawWidth(), getDrawHeight());
}

void MiniMapView::renderOneFrame(CL_GraphicContext& gc, const CL_Rect& clipRect) {
    CL_Draw::texture(gc, renderer_->getTexture(gc), CL_Rectf(0, 0, CL_Sizef(getWidth(), getHeight())));

    CL_Draw::point(gc, 150, 150, CL_Colorf::white);
}

void MiniMapView::getRequiredSectors(std::set<IsoIndex>& list, unsigned int mapHeight, unsigned int cacheAdd) {
    // cache which sectors we want to draw, there might be some more loaded
    sectorDrawSet_.clear();

    // at least, we need to load as much tiles as the diagonal of the view is long
    // we load this amount of tiles (plus a little cache) in each direction of the center tile

    double viewDiagonalPixel = sqrt(pow((float)getDrawWidth(), 2) + pow((float)getDrawHeight(), 2));

    double viewDiagonalSectorCount = viewDiagonalPixel / 8;
    int loadInEachDirection = (int)ceil(viewDiagonalSectorCount / 2);

    loadInEachDirection += cacheAdd; // some cache

    //LOG_DEBUG << "getRequiredSectors: diagonalPixel=" << viewDiagonalPixel << " diagonalTile=" << viewDiagonalTileCount <<
            //" diagonalSector=" << viewDiagonalSectorCount << " inEachDir=" << loadInEachDirection << std::endl;

    int centerSectorX = (int)(getCenterTileX() / 8.0);
    int centerSectorY = (int)(getCenterTileY() / 8.0);

    list.insert(IsoIndex(centerSectorX, centerSectorY));
    sectorDrawSet_.insert(IsoIndex(centerSectorX, centerSectorY));

    // uncomment this to load just a single sector
    //return;

    unsigned int sectorX, sectorY;
    int diff;
    for (int x = -loadInEachDirection; x <= loadInEachDirection; ++x) {
        diff = loadInEachDirection - abs(x);
        for (int y = -diff; y <= diff; ++y) {
            sectorX = (std::max)(centerSectorX + x, 0);
            sectorY = (std::max)(centerSectorY + y, 0);
            IsoIndex idx(sectorX, sectorY);
            list.insert(idx);
            sectorDrawSet_.insert(idx);
        }
    }
}

bool MiniMapView::shouldDrawSector(const IsoIndex& idx) const {
    return sectorDrawSet_.count(idx) > 0;
}

void MiniMapView::setCenterObject(boost::shared_ptr<world::IngameObject> obj) {
    centerObject_ = obj;
}

bool MiniMapView::onPointerExit() {
    ui::Manager::getCursorManager()->setCursorEnableFlags(ui::CursorEnableFlags::NONE);

    return true;
}

bool MiniMapView::onPointerEnter() {
    ui::Manager::getCursorManager()->setCursorEnableFlags(ui::CursorEnableFlags::ALL);
    return true;
}

CL_Mat4f MiniMapView::getViewMatrix() const {
    CL_Vec2f topLeft = getTopLeftPixel();

    CL_Mat4f ret = CL_Mat4f::scale(zoom_, zoom_, 0);

    ret.translate_self(getDrawWidth()/2., getDrawHeight()/2., 0.);
    ret.multiply(CL_Mat4f::rotate(CL_Angle::from_degrees(45.0), 0, 0, 1.0));
    ret.translate_self(getDrawWidth()/-2., getDrawHeight()/-2., 0.);

    ret.translate_self(-topLeft.x, -topLeft.y, 0);

    return ret;
}

void MiniMapView::forceRepaint() {
    renderer_->forceRepaint();
}

}
}
}
