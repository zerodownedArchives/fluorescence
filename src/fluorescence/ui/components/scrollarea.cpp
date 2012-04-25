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



#include "scrollarea.hpp"

#include <misc/log.hpp>

namespace fluo {
namespace ui {
namespace components {

ScrollArea::ScrollArea(CL_GUIComponent* parent) : CL_GUIComponent(parent), lastScrollVertical_(0), lastScrollHorizontal_(0) {
    horizontalScrollBar_ = new CL_ScrollBar(this);
    horizontalScrollBar_->set_horizontal();
    horizontalScrollBar_->set_position(0);
    horizontalScrollBar_->func_scroll().set(this, &ScrollArea::onScroll);

    verticalScrollBar_ = new CL_ScrollBar(this);
    verticalScrollBar_->set_vertical();
    verticalScrollBar_->set_position(0);
    verticalScrollBar_->func_scroll().set(this, &ScrollArea::onScroll);

    clientArea_ = new CL_Frame(this);
}

CL_ScrollBar* ScrollArea::getVerticalScrollBar() {
    return verticalScrollBar_;
}

CL_ScrollBar* ScrollArea::getHorizontalScrollBar() {
    return horizontalScrollBar_;
}

CL_Frame* ScrollArea::getClientArea() {
    return clientArea_;
}

void ScrollArea::updateScrollbars() {
    updateScrollbars(
            verticalVisibility_, horizontalVisibility_,
            verticalPageStep_, horizontalPageStep_,
            verticalLineStep_, horizontalLineStep_,
            marginLeft_, marginBottom_
    );
}

void ScrollArea::updateScrollbars(unsigned int verticalVisibility, unsigned int horizontalVisibility,
        unsigned int verticalPageStep, unsigned int horizontalPageStep,
        unsigned int verticalLineStep, unsigned int horizontalLineStep,
        unsigned int marginLeft, unsigned int marginBottom) {

    horizontalScrollBar_->set_min(0);
    verticalScrollBar_->set_min(0);

    int viewSizeWidth = get_width();
    int viewSizeHeight = get_height();

    // first, calculate values for page step and line step
    int childWidth = 0;
    int childHeight = 0;

    std::vector<CL_GUIComponent*> children = clientArea_->get_child_components();
    std::vector<CL_GUIComponent*>::const_iterator iter = children.begin();
    std::vector<CL_GUIComponent*>::const_iterator end = children.end();
    for (; iter != end; ++iter) {
        CL_Pointx<int> cur = (*iter)->get_geometry().get_bottom_right();
        cur.x += lastScrollHorizontal_;
        cur.y += lastScrollVertical_;
        if (cur.x > childWidth) {
            childWidth = cur.x;
        }

        if (cur.y > childHeight) {
            childHeight = cur.y;
        }
    }

    childWidth += marginLeft;
    childHeight += marginBottom;

    bool vVisible = true;
    bool hVisible = true;

    // this has to run twice, because the visibility of one scrollbar affects the other
    for (unsigned int i = 0; i < 2; ++i) {
        switch (verticalVisibility) {
        case VISIBLE_ALWAYS:
            vVisible = true;
            break;
        case VISIBLE_NEVER:
            vVisible = false;
            break;
        case VISIBLE_ON_DEMAND:
            vVisible = viewSizeHeight < childHeight;
            break;
        }

        switch (horizontalVisibility) {
        case VISIBLE_ALWAYS:
            hVisible = true;
            break;
        case VISIBLE_NEVER:
            hVisible = false;
            break;
        case VISIBLE_ON_DEMAND:
            hVisible = viewSizeWidth < childWidth;
            break;
        }

        if (vVisible) {
            unsigned int vWidth = verticalScrollBar_->get_preferred_width();
            unsigned int vHeight = get_height();
            if (hVisible) {
                vHeight -= horizontalScrollBar_->get_preferred_height();
            }
            unsigned int vX = get_width() - vWidth;
            unsigned int vY = 0;

            verticalScrollBar_->set_geometry(CL_Rect(vX, vY, CL_Size(vWidth, vHeight)));

            viewSizeWidth = get_width() - verticalScrollBar_->get_width();
        }

        if (hVisible) {
            unsigned int hWidth = get_width();
            if (vVisible) {
                hWidth -= verticalScrollBar_->get_preferred_width();
            }
            unsigned int hHeight = horizontalScrollBar_->get_preferred_height();
            unsigned int hX = 0;
            unsigned int hY = get_height() - hHeight;

            horizontalScrollBar_->set_geometry(CL_Rect(hX, hY, CL_Size(hWidth, hHeight)));

            viewSizeHeight = get_height() - horizontalScrollBar_->get_height();
        }
    }

    clientArea_->set_geometry(CL_Rect(0, 0, CL_Size(viewSizeWidth, viewSizeHeight)));

    horizontalScrollBar_->calculate_ranges(viewSizeWidth, childWidth);
    verticalScrollBar_->calculate_ranges(viewSizeHeight, childHeight);


    // overwrite values if parameter was given
    if (verticalPageStep) {
        verticalScrollBar_->set_page_step(verticalPageStep);
    }

    if (horizontalPageStep) {
        horizontalScrollBar_->set_page_step(horizontalPageStep);
    }

    if (verticalLineStep) {
        verticalScrollBar_->set_line_step(verticalLineStep);
    } else {
        int lineStep = verticalScrollBar_->get_page_step() / 10;
        lineStep = (std::max)(1, lineStep);
        verticalScrollBar_->set_line_step(lineStep);
    }

    if (horizontalLineStep) {
        horizontalScrollBar_->set_line_step(horizontalLineStep);
    } else {
        int lineStep = horizontalScrollBar_->get_page_step() / 10;
        lineStep = (std::max)(1, lineStep);
        horizontalScrollBar_->set_line_step(lineStep);
    }

    clientArea_->set_clip_children(true);
    verticalScrollBar_->set_visible(vVisible);
    horizontalScrollBar_->set_visible(hVisible);

    // if scrollbar is visible but not needed, disable
    if (hVisible) {
        if (childWidth <= viewSizeWidth) {
            horizontalScrollBar_->set_enabled(false);
        } else {
            horizontalScrollBar_->set_enabled(true);
        }
    }

    if (vVisible) {
        if (childHeight <= viewSizeHeight) {
            verticalScrollBar_->set_enabled(false);
        } else {
            verticalScrollBar_->set_enabled(true);
        }
    }
    
    
    LOG_DEBUG << "update scroll area: "
            << "child height=" << childHeight
            << " child width=" << childWidth
            << std::endl;
    
    
    // save parameters for next call
    verticalVisibility_ = verticalVisibility;
    horizontalVisibility_ = horizontalVisibility;
    verticalPageStep_ = verticalPageStep;
    horizontalPageStep_ = horizontalPageStep;
    verticalLineStep_ = verticalLineStep;
    horizontalLineStep_ = horizontalLineStep;
    marginLeft_ = marginLeft;
    marginBottom_ = marginBottom;
}

void ScrollArea::onScroll() {
    //LOGARG_DEBUG(LOGTYPE_UI, "onScroll vertical=%i horizontal=%i", verticalScrollBar_->get_position(), horizontalScrollBar_->get_position());

    int curScrollVert = verticalScrollBar_->get_position();
    int curScrollHori = horizontalScrollBar_->get_position();

    int diffVert = curScrollVert - lastScrollVertical_;
    int diffHori = curScrollHori - lastScrollHorizontal_;

    std::vector<CL_GUIComponent*> children = clientArea_->get_child_components();
    std::vector<CL_GUIComponent*>::const_iterator iter = children.begin();
    std::vector<CL_GUIComponent*>::const_iterator end = children.end();
    for (; iter != end; ++iter) {
        CL_Rect curGeom = (*iter)->get_geometry();
        CL_Rect newGeom = CL_Rect(curGeom.left - diffHori, curGeom.top - diffVert, curGeom.get_size());
        (*iter)->set_geometry(newGeom);
    }

    lastScrollVertical_ = curScrollVert;
    lastScrollHorizontal_ = curScrollHori;
}

void ScrollArea::setupResizeHandler() {
    std::vector<CL_GUIComponent*> children = clientArea_->get_child_components();
    std::vector<CL_GUIComponent*>::const_iterator iter = children.begin();
    std::vector<CL_GUIComponent*>::const_iterator end = children.end();
    for (; iter != end; ++iter) {
        (*iter)->func_resized().set(this, &ScrollArea::onChildComponentResized);
    }
}

void ScrollArea::onChildComponentResized() {
    updateScrollbars();
}

}
}
}
