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


#ifndef FLUO_UI_CURSORIMAGE_HPP
#define FLUO_UI_CURSORIMAGE_HPP

#include <boost/shared_ptr.hpp>

#ifdef WIN32
#include <ClanLib/Display/Window/cursor.h>
#else
#include <X11/Xlib.h>
#endif

class CL_DisplayWindow;

namespace fluo {
namespace ui {

class Texture;

class CursorImage {
public:
    CursorImage();
    ~CursorImage();

    void set(unsigned int cursorId, unsigned int artId, boost::shared_ptr<CL_DisplayWindow> window);

    void activate();

private:
    unsigned int cursorId_;

    int hotspotX_;
    int hotspotY_;

    unsigned int width_;
    unsigned int height_;

    void setHotspotFromTexture(boost::shared_ptr<ui::Texture> texture);

// on windows, we can just use the cursor thingy provided by clanlib.
// on unix, this does not work (yet), so we have to set the cursor directly
// windows stuff untested - not even compiled yet
#ifdef WIN32
    CL_Cursor winCursor_;
#else
    Cursor unixCursor_;
#endif

};

}
}

#endif
