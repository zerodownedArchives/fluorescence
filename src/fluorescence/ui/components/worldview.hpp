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


#ifndef FLUO_UI_WORLDVIEW_HPP
#define FLUO_UI_WORLDVIEW_HPP

#include <boost/shared_ptr.hpp>

#include <list>

#include <ui/gumpelement.hpp>

namespace fluo {

namespace world {
class IngameObject;
}

namespace ui {

class WorldViewRenderer;

namespace components {

class WorldView : public GumpElement {
public:
    WorldView(CL_GUIComponent* parent, const CL_Rect& bounds);
    ~WorldView();

    void setCenterObject(boost::shared_ptr<world::IngameObject> obj);

    void setCenterTiles(float x, float y);

    int getCenterPixelX();
    int getCenterPixelY();
    int getCenterPixelZ();

    unsigned int getWidth();
    unsigned int getHeight();

    void renderOneFrame(CL_GraphicContext& gc, const CL_Rect& clipRect);

    /// store all sectors this view needs (including some cache) in the list
    void getRequiredSectors(std::list<unsigned int>& list, unsigned int mapHeight, unsigned int cacheAdd);

    boost::shared_ptr<world::IngameObject> getFirstIngameObjectAt(unsigned int pixelX, unsigned int pixelY);

private:
    float centerTileX_;
    float centerTileY_;
    float centerTileZ_;

    boost::shared_ptr<world::IngameObject> centerObject_;

    float getCenterTileX();
    float getCenterTileY();
    float getCenterTileZ();

    boost::shared_ptr<WorldViewRenderer> renderer_;

    bool onInputPressed(const CL_InputEvent& e);
    bool onInputReleased(const CL_InputEvent & e);
    bool onDoubleClick(const CL_InputEvent& e);
};

}
}
}

#endif
