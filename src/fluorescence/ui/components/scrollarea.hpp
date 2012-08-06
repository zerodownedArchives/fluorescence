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


#ifndef FLUO_UI_SCROLLAREA_HPP
#define FLUO_UI_SCROLLAREA_HPP

#include <ui/gumpcomponent.hpp>

namespace fluo {
namespace ui {
namespace components {

class ScrollBar;

class ScrollArea : public GumpComponent {
public:
    ScrollArea(CL_GUIComponent* parent);

    GumpComponent* getClientArea();
    ScrollBar* getVerticalScrollBar();
    ScrollBar* getHorizontalScrollBar();

    void updateScrollbars(unsigned int marginRight, unsigned int marginBottom);

    // update scrollbars again, using the same parameters as the last call
    void updateScrollbars();

    // whenever a child component is resized, the scroll area has to be notified (and possibly change scrollbars)
    void setupResizeHandler();
    void onChildComponentResized();

private:
    ScrollBar* verticalScrollBar_;
    ScrollBar* horizontalScrollBar_;

    GumpComponent* clientArea_;

    int lastScrollVertical_;
    int lastScrollHorizontal_;
    void onScroll();

    unsigned int marginRight_;
    unsigned int marginBottom_;
};

}
}
}

#endif
