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


#ifndef FLUO_UI_CURSORMANAGER_HPP
#define FLUO_UI_CURSORMANAGER_HPP

#include <boost/shared_ptr.hpp>

#include <misc/config.hpp>

#include "cursorimage.hpp"

#include <world/ingameobject.hpp>

class CL_DisplayWindow;

namespace fluo {
namespace ui {

namespace targeting {
    class Target;
}

struct CursorType {
    enum {
        GAME_NORTHWEST = 0,
        GAME_NORTH = 1,
        GAME_NORTHEAST = 2,
        GAME_EAST = 3,
        GAME_SOUTHEAST = 4,
        GAME_SOUTH = 5,
        GAME_SOUTHWEST = 6,
        GAME_WEST = 7,
        FIST = 8,
        SELECTION = 9,
        PICK_UP = 10,
        BONE_HAND = 11,
        TARGET = 12,
        LOADING = 13,
        FEATHER = 14,
        NEEDLE = 15,

        COUNT,
    };
};

class CursorManager {
public:
    CursorManager(Config& config, boost::shared_ptr<CL_DisplayWindow> window);

    void setWarMode(bool value);
    void setCursor(unsigned int id);

    bool isDragging() const;
    void setDragCandidate(boost::shared_ptr<world::IngameObject> itm, int mouseX, int mouseY);
    boost::shared_ptr<world::IngameObject> stopDragging();

    void onCursorMove(const CL_Point& mousePos);

    void setTarget(boost::shared_ptr<targeting::Target> targ);
    void cancelTarget();
    bool hasTarget() const;
    bool onTarget(boost::shared_ptr<world::IngameObject> obj);

private:
    bool warMode_;
    unsigned int warmodeArtOffset_;
    unsigned int currentCursorId_;

    CursorImage cursorImages_[CursorType::COUNT * 2];

    void setCursorImage(unsigned int id, unsigned int artId, bool warMode, boost::shared_ptr<CL_DisplayWindow> window);

    void updateCursor();

    void startDragging();
    bool isDragging_;
    int dragStartMouseX_;
    int dragStartMouseY_;
    boost::shared_ptr<world::IngameObject> dragCandidate_;

    boost::shared_ptr<targeting::Target> target_;
};

}
}

#endif
