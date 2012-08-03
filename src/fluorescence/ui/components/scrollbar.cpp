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

/*
 * This file is based on (and partly copied from ClanLib's CL_ScrollBar component.
 *
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries ClanLib may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    Harry Storbacka
**    Magnus Norddahl
**    Kenneth Gangstoe
*/

#include "scrollbar.hpp"

#include <ClanLib/Core/Math/cl_math.h>
#include <ClanLib/Display/Window/keys.h>
#include <ClanLib/GUI/gui_message_pointer.h>
#include <ClanLib/Display/2D/draw.h>

#include <misc/log.hpp>

namespace fluo {
namespace ui {
namespace components {

ScrollBar::ScrollBar(CL_GUIComponent* parent) : GumpComponent(parent),
    incrementIndex_(0), decrementIndex_(0), thumbIndex_(0), visibility_(VISIBLE_ON_DEMAND),
    vertical(false), scroll_min(0), scroll_max(1), line_step(1),
    page_step(10), position(0), mouse_down_mode(mouse_down_none),
    thumb_start_position(0), thumb_start_pixel_position(0) {

    for (unsigned int i = 0; i < 3; ++i) {
        incrementColors_[i] = CL_Colorf::white;
        incrementHueInfos_[i] = CL_Vec3f(0, 0, 1);

        decrementColors_[i] = CL_Colorf::white;
        decrementHueInfos_[i] = CL_Vec3f(0, 0, 1);

        thumbColors_[i] = CL_Colorf::white;
        thumbHueInfos_[i] = CL_Vec3f(0, 0, 1);
    }

    trackColor_ = CL_Colorf::white;
    trackHueInfo_ = CL_Vec3f(0, 0, 1);

    set_type_name("ScrollBar");

    func_process_message().set(this, &ScrollBar::on_process_message);
	func_render().set(this, &ScrollBar::on_render);

	mouse_down_timer.func_expired().set(this, &ScrollBar::on_timer_expired);
}

ScrollBar::~ScrollBar() {
}

void ScrollBar::setTrackTexture(boost::shared_ptr<ui::Texture> texture, unsigned int hue, const std::string& rgba) {
    trackTexture_ = texture;

    if (rgba.length() > 0) {
        trackColor_ = CL_Colorf(rgba);
    }

    trackHueInfo_[1u] = hue;
}

void ScrollBar::setVisibility(unsigned int vis) {
    visibility_ = vis;
}

unsigned int ScrollBar::getVisibility() const {
    return visibility_;
}



// clanlib stuff

bool ScrollBar::is_vertical() const {
    return vertical;
}

bool ScrollBar::is_horizontal() const {
    return !vertical;
}

int ScrollBar::get_min() const {
    return scroll_min;
}

int ScrollBar::get_max() const {
    return scroll_max;
}

int ScrollBar::get_line_step() const {
    return line_step;
}

int ScrollBar::get_page_step() const {
    return page_step;
}

int ScrollBar::get_position() const {
    return position;
}

void ScrollBar::set_vertical() {
    vertical = true;
    if(update_part_positions())
        request_repaint();
}

void ScrollBar::set_horizontal() {
    vertical = false;
    if(update_part_positions())
        request_repaint();
}

void ScrollBar::set_min(int new_scroll_min) {
    set_ranges(new_scroll_min, scroll_max, line_step, page_step);
}

void ScrollBar::set_max(int new_scroll_max) {
    set_ranges(scroll_min, new_scroll_max, line_step, page_step);
}

void ScrollBar::set_line_step(int step) {
    set_ranges(scroll_min, scroll_max, step, page_step);
}

void ScrollBar::set_page_step(int step) {
    set_ranges(scroll_min, scroll_max, line_step, step);
}

void ScrollBar::set_ranges(int min, int max, int lstep, int pstep) {
    if (scroll_min >= scroll_max || line_step <= 0 || page_step <= 0)
        throw CL_Exception("Scrollbar ranges out of bounds!");
    scroll_min = min;
    scroll_max = max;
    line_step = lstep;
    page_step = pstep;
    if (position >= scroll_max)
        position = scroll_max-1;
    if (position < scroll_min)
        position = scroll_min;
    if(update_part_positions())
        request_repaint();
}

void ScrollBar::calculate_ranges(int view_size, int total_size) {
    if (view_size <= 0 || total_size <= 0) {
        set_ranges(0, 1, 1, 1);
    } else {
        int scroll_max = cl_max(1, total_size - view_size + 1);
        int page_step = cl_max(1, view_size);
        set_ranges(0, scroll_max, 1, page_step);
    }
}

void ScrollBar::set_position(int pos) {
    position = pos;
    if (pos >= scroll_max)
        position = scroll_max-1;
    if (pos < scroll_min)
        position = scroll_min;

    if(update_part_positions())
        request_repaint();
}

void ScrollBar::on_process_message(CL_GUIMessage &msg) {
    if (!is_enabled())
        return;

    if (msg.is_type(CL_GUIMessage_Input::get_type_name())) {
        CL_GUIMessage_Input input = msg;
        CL_InputEvent input_event = input.get_event();
        if (input_event.type == CL_InputEvent::pointer_moved)
            on_mouse_move(input, input_event);
        else if (input_event.type == CL_InputEvent::pressed && input_event.id == CL_MOUSE_LEFT)
            on_mouse_lbutton_down(input, input_event);
        else if (input_event.type == CL_InputEvent::released && input_event.id == CL_MOUSE_LEFT)
            on_mouse_lbutton_up(input, input_event);
    } else if (msg.is_type(CL_GUIMessage_Pointer::get_type_name())) {
        CL_GUIMessage_Pointer pointer = msg;
        if (pointer.get_pointer_type() == CL_GUIMessage_Pointer::pointer_leave)
            on_mouse_leave();
    }
}

void ScrollBar::on_mouse_move(CL_GUIMessage_Input &input, CL_InputEvent &input_event) {
    CL_Point pos = input_event.mouse_pos;

    updateTextureIndices(pos);

    if (mouse_down_mode == mouse_down_thumb_drag) {
        CL_Rect geom = get_window_geometry();

        int last_position = position;

        if (pos.x < -100 || pos.x > geom.get_width() + 100 || pos.y < -100 || pos.y > geom.get_height() + 100) {
            position = thumb_start_position;
        } else {
            int delta = vertical ? (pos.y - mouse_drag_start_pos.y) : (pos.x - mouse_drag_start_pos.x);
            int position_pixels = thumb_start_pixel_position + delta;

            int track_height = 0;
            if (vertical)
                track_height = rect_track_decrement.get_height()+rect_track_increment.get_height();
            else
                track_height = rect_track_decrement.get_width()+rect_track_increment.get_width();

            if (track_height != 0)
                position = scroll_min + position_pixels*(scroll_max-scroll_min)/track_height;
            else
                position = 0;

            if (position >= scroll_max)
                position = scroll_max-1;
            if (position < scroll_min)
                position = scroll_min;

        }

        if (position != last_position) {
            invoke_scroll_event();

            update_part_positions();
        }
    }

    request_repaint();

    input.set_consumed();
}

void ScrollBar::on_mouse_lbutton_down(CL_GUIMessage_Input &input, CL_InputEvent &input_event) {
    CL_Point pos = input_event.mouse_pos;
    mouse_drag_start_pos = pos;

    if (rect_button_decrement.contains(pos)) {
        mouse_down_mode = mouse_down_button_decr;

        int last_position = position;

        position -= line_step;
        last_step_size = -line_step;
        if (position >= scroll_max)
            position = scroll_max-1;
        if (position < scroll_min)
            position = scroll_min;

        if (last_position != position)
            invoke_scroll_event();
    } else if (rect_button_increment.contains(pos)) {
        mouse_down_mode = mouse_down_button_incr;

        int last_position = position;

        position += line_step;
        last_step_size = line_step;
        if (position >= scroll_max)
            position = scroll_max-1;
        if (position < scroll_min)
            position = scroll_min;

        if (last_position != position)
            invoke_scroll_event();
    } else if (rect_thumb.contains(pos)) {
        mouse_down_mode = mouse_down_thumb_drag;
        thumb_start_position = position;
        thumb_start_pixel_position = vertical ? (rect_thumb.top-rect_track_decrement.top) : (rect_thumb.left-rect_track_decrement.left);
    } else if (rect_track_decrement.contains(pos)) {
        mouse_down_mode = mouse_down_track_decr;

        int last_position = position;

        position -= page_step;
        last_step_size = -page_step;
        if (position >= scroll_max)
            position = scroll_max-1;
        if (position < scroll_min)
            position = scroll_min;

        if (last_position != position)
            invoke_scroll_event();
    } else if (rect_track_increment.contains(pos)) {
        mouse_down_mode = mouse_down_track_incr;

        int last_position = position;

        position += page_step;
        last_step_size = page_step;
        if (position >= scroll_max)
            position = scroll_max-1;
        if (position < scroll_min)
            position = scroll_min;

        if (last_position != position)
            invoke_scroll_event();
    }

    mouse_down_timer.start(100,false);

    update_part_positions();
    updateTextureIndices(pos);

    request_repaint();
    capture_mouse(true);
    input.set_consumed();
}

void ScrollBar::on_mouse_lbutton_up(CL_GUIMessage_Input &input, CL_InputEvent &input_event) {
    mouse_down_mode = mouse_down_none;
    mouse_down_timer.stop();

    updateTextureIndices(input_event.mouse_pos);

    request_repaint();
    capture_mouse(false);
    input.set_consumed();
}

void ScrollBar::on_mouse_leave() {
    updateTextureIndices(CL_Point(-1, -1));
    request_repaint();
}

void ScrollBar::on_render(CL_GraphicContext &gc, const CL_Rect &update_rect) {
    if (!incrementTextures_[incrementIndex_]->isReadComplete() ||
            !decrementTextures_[decrementIndex_]->isReadComplete() ||
            !thumbTextures_[thumbIndex_]->isReadComplete() ||
            !trackTexture_->isReadComplete()) {
        request_repaint();
        return;
    }
    // TODO

    CL_Draw::fill(gc, CL_Rect(0, 0, get_geometry().get_size()), CL_Colorf::red);
    CL_Draw::fill(gc, rect_button_decrement, CL_Colorf::blue);
    CL_Draw::fill(gc, rect_button_increment, CL_Colorf::blue);
    CL_Draw::fill(gc, rect_thumb, CL_Colorf::green);

    //CL_Rect rect = get_geometry();
    //part_component.render_box(gc, rect.get_size(), update_rect);
    //part_button_decrement.render_box(gc, rect_button_decrement, update_rect);
    //part_track_decrement.render_box(gc, rect_track_decrement, update_rect);
    //part_thumb.render_box(gc, rect_thumb, update_rect);
    //part_thumb_gripper.render_box(gc, rect_thumb, update_rect);
    //part_track_increment.render_box(gc, rect_track_increment, update_rect);
    //part_button_increment.render_box(gc, rect_button_increment, update_rect);
}

// Calculates positions of all parts. Returns true if thumb position was changed compared to previously, false otherwise.
bool ScrollBar::update_part_positions() {
    // textures not set yet
    if (!trackTexture_) {
        return false;
    }

    CL_Rect rect(CL_Point(0,0), get_geometry().get_size());

    int decr_height = decrementTextures_[decrementIndex_]->getHeight();
    int incr_height = incrementTextures_[incrementIndex_]->getHeight();
    int total_height = vertical ? rect.get_height() : rect.get_width();
    int track_height = cl_max(0, total_height - decr_height - incr_height);
    int thumb_height = thumbTextures_[thumbIndex_]->getHeight(); //calculate_thumb_size(track_height);

    int thumb_offset = decr_height + calculate_thumb_position(thumb_height, track_height);

    CL_Rect previous_rect_thumb = rect_thumb;

    rect_button_decrement = create_rect(rect, 0, decr_height);
    rect_track_decrement = create_rect(rect, decr_height, thumb_offset);
    rect_thumb = create_rect(rect, thumb_offset, thumb_offset+thumb_height);
    rect_track_increment = create_rect(rect, thumb_offset+thumb_height, decr_height+track_height);
    rect_button_increment = create_rect(rect, decr_height+track_height, decr_height+track_height+incr_height);

    return (previous_rect_thumb != rect_thumb);
}

int ScrollBar::calculate_thumb_size(int track_size) {
    int minimum_thumb_size = 20;
    int range = scroll_max-scroll_min;
    int length = range+page_step-1;
    int thumb_size = page_step*track_size/length;
    if (thumb_size < minimum_thumb_size)
        thumb_size = minimum_thumb_size;
    if (thumb_size > track_size)
        thumb_size = track_size;
    return thumb_size;
}

int ScrollBar::calculate_thumb_position(int thumb_size, int track_size) {
    int relative_pos = position-scroll_min;
    int range = scroll_max-scroll_min-1;
    if (range != 0) {
        int available_area = cl_max(0, track_size-thumb_size);
        return relative_pos*available_area/range;
    } else {
        return 0;
    }
}

CL_Rect ScrollBar::create_rect(const CL_Rect content_rect, int start, int end) {
    if (vertical)
        return CL_Rect(content_rect.left, content_rect.top+start, content_rect.right, content_rect.top+end);
    else
        return CL_Rect(content_rect.left+start, content_rect.top, content_rect.left+end, content_rect.bottom);
}

void ScrollBar::on_timer_expired() {
    if (mouse_down_mode == mouse_down_thumb_drag)
        return;

    mouse_down_timer.start(100, false);

    int last_position = position;
    position += last_step_size;
    if (position >= scroll_max)
        position = scroll_max-1;

    if (position < scroll_min)
        position = scroll_min;

    if (position != last_position) {
        invoke_scroll_event();

        if(update_part_positions())
            request_repaint();
    }
}

void ScrollBar::invoke_scroll_event() {
    if (!func_scroll_.is_null())
		func_scroll_.invoke();
}

void ScrollBar::updateTextureIndices(const CL_Point& pos) {
    if (pos.x == -1 && pos.y == -1) {
        incrementIndex_ = TEX_INDEX_UP;
        decrementIndex_ = TEX_INDEX_UP;
        thumbIndex_ = TEX_INDEX_UP;
    } else {
        thumbIndex_ = rect_thumb.contains(pos) ? (mouse_down_mode == mouse_down_none ? TEX_INDEX_MOUSEOVER : TEX_INDEX_DOWN) : TEX_INDEX_UP;
        incrementIndex_ = rect_button_increment.contains(pos) ? (mouse_down_mode == mouse_down_none ? TEX_INDEX_MOUSEOVER : TEX_INDEX_DOWN) : TEX_INDEX_UP;
        decrementIndex_ = rect_button_decrement.contains(pos) ? (mouse_down_mode == mouse_down_none ? TEX_INDEX_MOUSEOVER : TEX_INDEX_DOWN) : TEX_INDEX_UP;
    }
}

CL_Callback_v0& ScrollBar::func_scroll() {
	return func_scroll_;
}

}
}
}
