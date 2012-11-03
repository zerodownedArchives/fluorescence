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

#include "scrollbar.hpp"

#include <misc/log.hpp>

namespace fluo {
namespace ui {
namespace components {

ScrollArea::ScrollArea(CL_GUIComponent* parent) : GumpComponent(parent), lastScrollVertical_(0), lastScrollHorizontal_(0) {
    horizontalScrollBar_ = new ScrollBar(this);
    horizontalScrollBar_->set_horizontal();
    horizontalScrollBar_->set_position(0);
    horizontalScrollBar_->func_scroll().set(this, &ScrollArea::onScroll);

    verticalScrollBar_ = new ScrollBar(this);
    verticalScrollBar_->set_vertical();
    verticalScrollBar_->set_position(0);
    verticalScrollBar_->func_scroll().set(this, &ScrollArea::onScroll);

    clientArea_ = new GumpComponent(this);
    clientArea_->set_type_name("scrollarea-client");

    set_type_name("scrollarea");
}

ScrollBar* ScrollArea::getVerticalScrollBar() {
    return verticalScrollBar_;
}

ScrollBar* ScrollArea::getHorizontalScrollBar() {
    return horizontalScrollBar_;
}

GumpComponent* ScrollArea::getClientArea() {
    return clientArea_;
}

void ScrollArea::setMargins(unsigned int marginLeft, unsigned int marginTop, unsigned int marginRight, unsigned int marginBottom) {
    marginLeft_ = marginLeft;
    marginTop_ = marginTop;
    marginRight_ = marginRight;
    marginBottom_ = marginBottom;
}

void ScrollArea::updateScrollbars() {
    horizontalScrollBar_->set_min(0);
    verticalScrollBar_->set_min(0);

    int viewSizeWidth = get_width() - (marginLeft_ + marginRight_);
    int viewSizeHeight = get_height() - (marginTop_ + marginBottom_);

    // first, calculate values for page step and line step
    int childWidth = 0;
    int childHeight = 0;

    std::vector<CL_GUIComponent*> children = clientArea_->get_child_components();
    std::vector<CL_GUIComponent*>::const_iterator iter = children.begin();
    std::vector<CL_GUIComponent*>::const_iterator end = children.end();
    for (; iter != end; ++iter) {
        CL_Pointx<int> cur = (*iter)->get_geometry().get_bottom_right();
        if (cur.x > childWidth) {
            childWidth = cur.x;
        }

        if (cur.y > childHeight) {
            childHeight = cur.y;
        }
    }

    bool vVisible = verticalScrollBar_->is_visible();
    bool hVisible = horizontalScrollBar_->is_visible();

    if (vVisible) {
        unsigned int vWidth = verticalScrollBar_->get_width();
        unsigned int vHeight = get_height();
        if (hVisible) {
            vHeight -= horizontalScrollBar_->get_height();
        }
        unsigned int vX = get_width() - vWidth;
        unsigned int vY = 0;

        verticalScrollBar_->set_geometry(CL_Rect(vX, vY, CL_Size(vWidth, vHeight)));

        viewSizeWidth -= verticalScrollBar_->get_width();
    }

    if (hVisible) {
        unsigned int hWidth = get_width();
        if (vVisible) {
            hWidth -= verticalScrollBar_->get_width();
        }
        unsigned int hHeight = horizontalScrollBar_->get_height();
        unsigned int hX = 0;
        unsigned int hY = get_height() - hHeight;

        horizontalScrollBar_->set_geometry(CL_Rect(hX, hY, CL_Size(hWidth, hHeight)));

        viewSizeHeight -= horizontalScrollBar_->get_height();
    }

    clientArea_->set_geometry(CL_Rect(marginLeft_, marginTop_, CL_Size(viewSizeWidth, viewSizeHeight)));

    horizontalScrollBar_->calculate_ranges(viewSizeWidth, childWidth);
    verticalScrollBar_->calculate_ranges(viewSizeHeight, childHeight);

    clientArea_->set_clip_children(true);

    setupResizeHandler();
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


void ScrollArea::setMarginLeft(unsigned int margin) {
    marginLeft_ = margin;
}

unsigned int ScrollArea::getMarginLeft() const {
    return marginLeft_;
}

void ScrollArea::setMarginRight(unsigned int margin) {
    marginRight_ = margin;
}

unsigned int ScrollArea::getMarginRight() const {
    return marginRight_;
}

void ScrollArea::setMarginTop(unsigned int margin) {
    marginTop_ = margin;
}

unsigned int ScrollArea::getMarginTop() const {
    return marginTop_;
}

void ScrollArea::setMarginBottom(unsigned int margin) {
    marginBottom_ = margin;
}

unsigned int ScrollArea::getMarginBottom() const {
    return marginBottom_;
}

}
}
}
