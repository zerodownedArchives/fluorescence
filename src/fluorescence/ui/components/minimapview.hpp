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


#ifndef FLUO_UI_MINIMAPVIEW_HPP
#define FLUO_UI_MINIMAPVIEW_HPP

#include "sectorview.hpp"

#include <boost/shared_ptr.hpp>
#include <list>
#include <set>

#include <typedefs.hpp>
#include <ui/gumpcomponent.hpp>

namespace fluo {

namespace world {
class IngameObject;
}

namespace ui {

namespace render {
class MiniMapRenderer;
}

namespace components {

class MiniMapView : public GumpComponent, public SectorView {
public:
    MiniMapView(CL_GUIComponent* parent);

    void setCenterObject(boost::shared_ptr<world::IngameObject> obj);

    void setCenterTiles(float x, float y);

    float getCenterPixelX() const;
    float getCenterPixelY() const;
    CL_Vec2f getTopLeftPixel() const;

    unsigned int getWidth() const;
    unsigned int getHeight() const;

    // widht/height including zoom
    CL_Size getDrawSize() const;
    unsigned int getDrawWidth() const;
    unsigned int getDrawHeight() const;
    float getZoom() const;
    void zoomIn(float val);
    void zoomOut(float val);
    void zoomReset();

    void renderOneFrame(CL_GraphicContext& gc, const CL_Rect& clipRect);

    /// store all sectors this view needs (including some cache) in the list
    virtual void getRequiredSectors(std::set<IsoIndex>& list, unsigned int mapHeight, unsigned int cacheAdd);
    bool shouldDrawSector(const IsoIndex& idx) const;

    CL_Mat4f getViewMatrix() const;

    void forceRepaint();

private:
    float centerTileX_;
    float centerTileY_;
    float centerTileZ_;

    boost::shared_ptr<world::IngameObject> centerObject_;

    float getCenterTileX() const;
    float getCenterTileY() const;
    float getCenterTileZ() const;

    boost::shared_ptr<render::MiniMapRenderer> renderer_;

    bool onDoubleClick(const CL_InputEvent& e);
    bool onPointerMoved(const CL_InputEvent& e);
    bool onPointerEnter();
    bool onPointerExit();

    float zoom_;

    std::set<IsoIndex> sectorDrawSet_;
};

}
}
}

#endif
