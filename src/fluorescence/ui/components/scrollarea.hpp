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

#include <ClanLib/gui.h>

namespace fluo {
namespace ui {
namespace components {

class ScrollArea : public CL_GUIComponent {
public:
    static const unsigned int VISIBLE_ON_DEMAND = 0;
    static const unsigned int VISIBLE_ALWAYS = 1;
    static const unsigned int VISIBLE_NEVER = 2;

    ScrollArea(CL_GUIComponent* parent);

    CL_Frame* getClientArea();
    CL_ScrollBar* getVerticalScrollBar();
    CL_ScrollBar* getHorizontalScrollBar();

    /**
     * \brief Calculate scroll bar visibility and parameters
     * \param verticalPageStep 0 for auto, value to control vertical page step size directly
     * \param horizontalPageStep 0 for auto, value to control horizontal page step size directly
     */
    void updateScrollbars(unsigned int verticalVisibility = VISIBLE_ON_DEMAND, unsigned int horizontalVisibility = VISIBLE_ON_DEMAND,
            unsigned int verticalPageStep = 0, unsigned int horizontalPageStep = 0,
            unsigned int verticalLineStep = 0, unsigned int horizontalLineStep = 0,
            unsigned int marginLeft = 0, unsigned int marginBottom = 0);

private:
    CL_ScrollBar* verticalScrollBar_;
    CL_ScrollBar* horizontalScrollBar_;

    CL_Frame* clientArea_;

    void onScroll();
};

}
}
}

#endif
