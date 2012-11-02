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
 * This file is based on (and partly copied from ClanLib's CL_LineEdit component.
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
*/



#include "lineedit.hpp"

#include <algorithm>
#include <ClanLib/Core/Text/string_format.h>
#include <ClanLib/Core/Text/utf8_reader.h>
#include <ClanLib/GUI/gui_component.h>
#include <ClanLib/GUI/gui_message.h>
#include <ClanLib/GUI/gui_manager.h>
#include <ClanLib/GUI/gui_message_input.h>
#include <ClanLib/GUI/gui_message_pointer.h>
#include <ClanLib/GUI/gui_message_focus_change.h>
#include <ClanLib/Core/Math/cl_math.h>
#include <ClanLib/Core/System/timer.h>
#include <ClanLib/Core/Text/string_help.h>
#include <ClanLib/Display/Window/input_event.h>
#include <ClanLib/Display/Window/keys.h>
#include <ClanLib/Display/Font/font_metrics.h>
#include <ClanLib/Display/2D/draw.h>

#ifndef WIN32
#include "stdlib.h"
#endif

#include <boost/python/extract.hpp>

#include <data/manager.hpp>
#include <data/huesloader.hpp>
#include <misc/log.hpp>
#include <ui/manager.hpp>
#include <ui/gumpmenu.hpp>
#include <ui/python/scriptloader.hpp>

namespace fluo {
namespace ui {
namespace components {

LineEdit::LineEdit(CL_GUIComponent* parent) :
        GumpComponent(parent),
        entryId_(0xFFFFFFFFu),
        fontColor_(CL_Colorf::black),
        overrideGumpFont_(false),

        cursor_pos(0),
        max_length(-1),
        mouse_selecting(false),
        password_mode(false),
        numeric_mode(false),
        selection_start(-1),
        selection_length(0),
        decimal_char("."),
        cursor_drawing_enabled_when_parent_focused(false),
        mouse_moves_left(false),
        cursor_blink_visible(true),
        clip_start_offset(0),
        clip_end_offset(0),
        ignore_mouse_events(false) {

    set_focus_policy(focus_local);

    func_render().set(this, &LineEdit::on_render);
    func_process_message().set(this, &LineEdit::on_process_message);

    timer.func_expired().set(this, &LineEdit::on_timer_expired);
    scroll_timer.func_expired().set(this, &LineEdit::on_scroll_timer_expired);

    set_type_name("lineedit");
}

LineEdit::~LineEdit() {
    timer.stop();
    scroll_timer.stop();
}

UnicodeString LineEdit::getText() {
    return StringConverter::fromUtf8(get_text());
}

void LineEdit::setText(const UnicodeString& text) {
    set_text(StringConverter::toUtf8String(text));
}

void LineEdit::onEnterPressed() {
    if (pyEnterCallback_) {
        try {
            pyEnterCallback_(boost::python::ptr(this));
        } catch (boost::python::error_already_set const&) {
            ui::Manager::getPythonLoader()->logError();
        }
    }
}

void LineEdit::setEntryId(unsigned int entryId) {
    entryId_ = entryId;
}

unsigned int LineEdit::getEntryId() const {
    return entryId_;
}

void LineEdit::setFont(const UnicodeString& fontName, unsigned int fontHeight) {
    setFontB(fontName, fontHeight, false);
}

void LineEdit::setFontB(const UnicodeString& name, unsigned int height, bool border) {
    fontDesc_.set_typeface_name(StringConverter::toUtf8String(name));
    fontDesc_.set_height(height);
    fontDesc_.set_weight(border ? 700 : 400); // weight values taken from clanlib
    fontDesc_.set_subpixel(false);

    cachedFont_ = ui::Manager::getSingleton()->getFont(fontDesc_);
    overrideGumpFont_ = true;

    isUoFont_ = name.indexOf("unifont") != -1;
}

CL_Font LineEdit::getFont() const {
    return overrideGumpFont_ ? cachedFont_ : getGumpMenu()->getFont();
}

CL_Colorf LineEdit::getRgba() const {
    return fontColor_;
}

void LineEdit::setRgba(const CL_Colorf& rgba) {
    fontColor_ = rgba;
}

void LineEdit::setHue(unsigned int hue) {
    fontColor_ = data::Manager::getHuesLoader()->getFontClColor(hue);
}

void LineEdit::setRgba(float r, float g, float b, float a) {
    setRgba(CL_Colorf(r, g, b, a));
}

void LineEdit::setRgba(float r, float g, float b) {
    setRgba(CL_Colorf(r, g, b));
}

boost::python::tuple LineEdit::pyGetRgba() const {
    CL_Colorf rgba = getRgba();
    return boost::python::make_tuple(rgba.r, rgba.g, rgba.b, rgba.a);
}

void LineEdit::pySetRgba(const boost::python::tuple& rgba) {
    namespace bpy = boost::python;

    float r = bpy::extract<float>(rgba[0]);
    float g = bpy::extract<float>(rgba[1]);
    float b = bpy::extract<float>(rgba[2]);

    if (bpy::len(rgba) > 3) {
        float a = bpy::extract<float>(rgba[3]);
        setRgba(r, g, b, a);
    } else {
        setRgba(r, g, b);
    }
}

boost::python::object LineEdit::getPyEnterCallback() const {
    return pyEnterCallback_;
}

void LineEdit::setPyEnterCallback(boost::python::object cb) {
    pyEnterCallback_ = cb;
}

// clanlib stuff

const CL_StringRef LineEdit::numeric_mode_characters = "0123456789";

bool LineEdit::is_password_mode() const {
    return password_mode;
}

int LineEdit::get_max_length() const {
    return max_length;
}

CL_StringRef LineEdit::get_text() const {
    return text;
}

int LineEdit::getTextInt() const {
    return CL_StringHelp::text_to_int(text);
}

CL_String LineEdit::get_selection() const {
    int start = cl_min(selection_start, selection_start + selection_length);
    return text.substr(start, abs(selection_length));
}

int LineEdit::get_selection_start() const {
    return selection_start;
}

int LineEdit::get_selection_length() const {
    return selection_length;
}

int LineEdit::get_cursor_pos() const {
    return cursor_pos;
}

CL_Size LineEdit::get_text_size() {
    CL_GraphicContext &gc = get_gc();
    CL_Font font = getFont();
    return get_visual_text_size(gc, font);
}

CL_Size LineEdit::get_text_size(const CL_String &str) {
    CL_GraphicContext &gc = get_gc();
    CL_Size text_size = getFont().get_text_size(gc, str);
    return text_size;
}

void LineEdit::select_all() {
    set_text_selection(0, text.size());
    request_repaint();
}

void LineEdit::setPasswordMode(bool enable) {
    if (password_mode != enable) {
        password_mode = enable;
        request_repaint();
    }
}

bool LineEdit::getPasswordMode() const {
    return password_mode;
}

void LineEdit::setMaxLength(int length) {
    if (max_length != length) {
        max_length = length;
        if ((int)text.length() > length) {
            CL_InputEvent no_event;
            text = text.substr(0, length);
        }
        request_repaint();
    }
}

int LineEdit::getMaxLength() const {
    return max_length;
}

void LineEdit::set_text(const CL_StringRef &t) {
    text = t;

    clip_start_offset = 0;
    update_text_clipping();
    set_cursor_pos(text.size());
    set_text_selection(0, 0);
    request_repaint();
}

void LineEdit::set_text(int number) {
    text = CL_StringHelp::int_to_text(number);
    clip_start_offset = 0;
    update_text_clipping();
    set_cursor_pos(text.size());
    set_text_selection(0, 0);
    request_repaint();
}

void LineEdit::set_selection(int pos, int length) {
    //don't call func_selection_changed() here, because this
    //member is for public usage
    selection_start = pos;
    selection_length = length;
    request_repaint();
}

void LineEdit::clear_selection() {
    //don't call func_selection_changed() here, because this
    //member is for public usage
    set_selection(0, 0);
    request_repaint();
}

void LineEdit::delete_selected_text() {
    if (get_selection_length() == 0)
        return;

    int sel_start = selection_start;
    int sel_end = selection_start + selection_length;
    if (sel_start > sel_end)
        std::swap(sel_start, sel_end);

    text = text.substr(0, sel_start) + text.substr(sel_end, text.size());
    cursor_pos = sel_start;
    set_text_selection(0, 0);
    int old_pos = get_cursor_pos();
    set_cursor_pos(0);
    set_cursor_pos(old_pos);
}

void LineEdit::set_cursor_pos(int pos) {
    cursor_pos = pos;
    update_text_clipping();
    request_repaint();
}

/////////////////////////////////////////////////////////////////////////////
// CL_LineEdit Events:

void LineEdit::set_input_mask( const CL_StringRef &mask ) {
    input_mask = mask;
}

void LineEdit::setNumericMode( bool enable ) {
    numeric_mode = enable;
}

bool LineEdit::getNumericMode() const {
    return numeric_mode;
}

void LineEdit::set_decimal_character( const CL_StringRef &dc ) {
    decimal_char = dc;
}

void LineEdit::set_cursor_drawing_enabled(bool enable) {
    cursor_drawing_enabled_when_parent_focused = enable;

    timer.start(500);
}

/////////////////////////////////////////////////////////////////////////////
// CL_LineEdit Implementation:

void LineEdit::on_process_message(CL_GUIMessage &msg) {
    if (msg.is_type(CL_GUIMessage_Input::get_type_name())) {
        CL_GUIMessage_Input input_msg = msg;
        CL_InputEvent e = input_msg.get_event();

        if (e.device.get_type() == CL_InputDevice::keyboard) {
            if (e.type == CL_InputEvent::pressed &&
                    (e.id == CL_KEY_ENTER || e.id == CL_KEY_RETURN || e.id == CL_KEY_NUMPAD_ENTER) &&
                    pyEnterCallback_) {
                onEnterPressed();
                msg.set_consumed();
                return;
            }

            cursor_blink_visible = true;
            timer.start(500); // don't blink cursor when moving or typing.

            if (e.type == CL_InputEvent::pressed) { // || e.repeat_count > 1)
                if (e.id == CL_KEY_ENTER || e.id == CL_KEY_ESCAPE || e.id == CL_KEY_TAB || e.id == CL_KEY_NUMPAD_ENTER) {
                    // Do not consume these.
                    return;
                } else if (e.id == CL_KEY_A && e.ctrl) {
                    // select all
                    set_text_selection(0, text.size());
                    cursor_pos = selection_length;
                    update_text_clipping();
                    request_repaint();
                    msg.set_consumed();
                } else if (e.id == CL_KEY_C && e.ctrl) {
                    if(!password_mode) {    // Do not allow copying the password to clipboard
                        CL_String str = get_selection();
                        get_gui_manager().set_clipboard_text(str);
                        msg.set_consumed();
                    }
                } else if (e.id == CL_KEY_LEFT) {
                    move(-1, e);
                    msg.set_consumed();
                } else if (e.id == CL_KEY_RIGHT) {
                    move(1, e);
                    msg.set_consumed();
                } else if (e.id == CL_KEY_BACKSPACE) {
                    backspace();
                    update_text_clipping();
                    msg.set_consumed();
                } else if (e.id == CL_KEY_DELETE) {
                    del();
                    update_text_clipping();
                    msg.set_consumed();
                } else if (e.id == CL_KEY_HOME) {
                    set_selection_start(cursor_pos);
                    cursor_pos = 0;
                    if (e.shift)
                        set_selection_length(-selection_start);
                    else
                        set_text_selection(0, 0);
                    update_text_clipping();
                    request_repaint();
                    msg.set_consumed();
                } else if (e.id == CL_KEY_END) {
                    set_selection_start(cursor_pos);
                    cursor_pos = text.size();
                    if (e.shift)
                        set_selection_length(text.size() - selection_start);
                    else
                        set_text_selection(0, 0);
                    update_text_clipping();
                    request_repaint();
                    msg.set_consumed();
                } else if (e.id == CL_KEY_X && e.ctrl) {
                    CL_String str = get_selection();
                    delete_selected_text();
                    get_gui_manager().set_clipboard_text(str);
                    update_text_clipping();
                    msg.set_consumed();
                } else if (e.id == CL_KEY_V && e.ctrl) {
                    CL_String str = get_gui_manager().get_clipboard_text();
                    CL_String::const_iterator end_str = std::remove(str.begin(), str.end(), '\n');
                    str.resize(end_str - str.begin());
                    end_str = std::remove(str.begin(), str.end(), '\r');
                    str.resize(end_str - str.begin());
                    delete_selected_text();

                    if (input_mask.empty()) {
                        if (numeric_mode) {
                            CL_String present_text = get_text();

                            bool present_minus = present_text.find('-') != CL_String::npos;
                            bool str_minus = str.find('-') != CL_String::npos;

                            if(!present_minus || !str_minus) {
                                if((!present_minus && !str_minus) || //if no minus found
                                        (str_minus && cursor_pos == 0 && str[0] == '-') || //if there's minus in text to paste
                                        (present_minus && cursor_pos > 0)) { //if there's minus in the beginning of control's text
                                    if(str.find_first_not_of(numeric_mode_characters, str[0] == '-' ? 1 : 0) == CL_String::npos) {
                                        if(insert_text(cursor_pos, str))
                                            set_cursor_pos(cursor_pos + str.length());
                                    }
                                }
                            }
                        } else {
                            if(insert_text(cursor_pos, str))
                                set_cursor_pos(cursor_pos + str.length());
                        }
                    } else {
                        if (input_mask_accepts_input(cursor_pos, str)) {
                            if(insert_text(cursor_pos, str))
                                set_cursor_pos(cursor_pos + str.length());
                        }
                    }

                    update_text_clipping();
                    msg.set_consumed();
                } else if (e.id == CL_KEY_SHIFT) {
                    if (selection_start == -1)
                        set_text_selection(cursor_pos, 0);

                    msg.set_consumed();
                } else if (!e.str.empty() && !(e.str[0] >= 0 && e.str[0] < 32) && ((!e.alt && !e.ctrl) || (e.ctrl && e.alt))) { // Alt Gr translates to Ctrl+Alt sometimes!
                    delete_selected_text();
                    if (input_mask.empty()) {
                        if (numeric_mode) {
                            // '-' can only be added once, and only as the first character.
                            if (e.str == "-" && cursor_pos == 0 && text.find("-") == CL_StringRef::npos) {
                                if(insert_text(cursor_pos, e.str))
                                    cursor_pos += e.str.size();
                            } else if (numeric_mode_characters.find(e.str) != CL_StringRef::npos) { // 0-9
                                if(insert_text(cursor_pos, e.str))
                                    cursor_pos += e.str.size();
                            }
                        } else {
                            // not in any special mode, just insert the string.
                            if(insert_text(cursor_pos, e.str))
                                cursor_pos += e.str.size();
                        }
                    } else {
                        if (input_mask_accepts_input(cursor_pos, e.str)) {
                            if(insert_text(cursor_pos, e.str))
                                cursor_pos += e.str.size();
                        }
                    }
                    update_text_clipping();
                    msg.set_consumed();
                }
            } else if (e.type == CL_InputEvent::released) {
                // undo
                if (e.ctrl && e.id == CL_KEY_Z) {
                    CL_String tmp = undo_info.undo_text;
                    undo_info.undo_text = get_text();
                    set_text(tmp);
                    msg.set_consumed();
                }
            }

        } else if (e.device.get_type() == CL_InputDevice::pointer) {
            if (e.type == CL_InputEvent::pressed && e.id == CL_MOUSE_LEFT) {
                if (has_focus()) {
                    capture_mouse(true);
                    mouse_selecting = true;
                    cursor_pos = get_character_index(e.mouse_pos.x);
                    set_text_selection(cursor_pos, 0);
                } else {
                    set_focus();
                }
                request_repaint();
                msg.set_consumed();
            }
            if (mouse_selecting && e.type == CL_InputEvent::released && e.id == CL_MOUSE_LEFT) {
                if (ignore_mouse_events) { // This prevents text selection from changing from what was set when focus was gained.
                    capture_mouse(false);
                    ignore_mouse_events = false;
                    mouse_selecting = false;
                } else {
                    scroll_timer.stop();
                    capture_mouse(false);
                    mouse_selecting = false;
                    int sel_end = get_character_index(e.mouse_pos.x);
                    set_selection_length(sel_end - selection_start);
                    cursor_pos = sel_end;
                    set_focus();
                    request_repaint();
                }
                msg.set_consumed();
            }
            if (e.type == CL_InputEvent::pointer_moved && mouse_selecting && !ignore_mouse_events) {
                if (e.mouse_pos.x < 0 || e.mouse_pos.x > get_width()) {
                    if (e.mouse_pos.x < 0)
                        mouse_moves_left = true;
                    else
                        mouse_moves_left = false;

                    scroll_timer.start(50, true);
                } else {
                    scroll_timer.stop();
                    cursor_pos = get_character_index(e.mouse_pos.x);
                    set_selection_length(cursor_pos - selection_start);
                    request_repaint();
                }
                msg.set_consumed();
            }
        }
    } else if (msg.is_type(CL_GUIMessage_FocusChange::get_type_name())) {
        CL_GUIMessage_FocusChange fmsg(msg);

        if (fmsg.get_focus_type() == CL_GUIMessage_FocusChange::gained_focus) {
            if (get_focus_policy() == CL_GUIComponent::focus_parent) {
                get_parent_component()->set_focus();
                timer.start(500);
                return;
            }

            timer.start(500);
            ignore_mouse_events = true;
            cursor_pos = text.length();

            request_repaint();

        } else if (fmsg.get_focus_type() == CL_GUIMessage_FocusChange::losing_focus) {
            timer.stop();
            set_text_selection(0, 0);

            request_repaint();
        }
    } else if (msg.is_type(CL_GUIMessage_Pointer::get_type_name())) {
        CL_GUIMessage_Pointer pointer_msg(msg);
        if (pointer_msg.get_pointer_type() == CL_GUIMessage_Pointer::pointer_enter) {
            set_cursor(cl_cursor_ibeam);
        } else {
            set_cursor(cl_cursor_arrow);
        }
    }
}

void LineEdit::move(int steps, CL_InputEvent &e) {
    if (e.shift && selection_length == 0)
        set_selection_start(cursor_pos);

    // Jump over words if control is pressed.
    if (e.ctrl) {
        if (steps < 0)
            steps = find_previous_break_character(cursor_pos-1) - cursor_pos;
        else
            steps = find_next_break_character(cursor_pos+1) - cursor_pos;

        cursor_pos += steps;
        if (cursor_pos < 0)
            cursor_pos = 0;
        if (cursor_pos > (int)text.size())
            cursor_pos = text.size();
    } else {
        CL_UTF8_Reader utf8_reader(text);
        utf8_reader.set_position(cursor_pos);
        if (steps > 0) {
            for (int i = 0; i < steps; i++)
                utf8_reader.next();
        } else if (steps < 0) {
            for (int i = 0; i < -steps; i++)
                utf8_reader.prev();
        }

        cursor_pos = utf8_reader.get_position();
    }


    // Clear the selection if a cursor key is pressed but shift isn't down.
    if (e.shift)
        set_selection_length(cursor_pos - selection_start);
    else
        set_text_selection(-1, 0);

    update_text_clipping();

    request_repaint();

    undo_info.first_text_insert = true;
}

bool LineEdit::insert_text(int pos, const CL_StringRef &str) {
    undo_info.first_erase = false;
    if (undo_info.first_text_insert) {
        undo_info.undo_text = get_text();
        undo_info.first_text_insert = false;
    }

    // checking if insert exceeds max length
    if(text.utf8_length() + str.utf8_length() > (unsigned int)max_length) {
        return false;
    }

    text.insert(pos, str);

    update_text_clipping();
    request_repaint();
    return true;
}

void LineEdit::backspace() {
    if (undo_info.first_erase) {
        undo_info.first_erase = false;
        undo_info.undo_text = get_text();
    }

    if (get_selection_length() != 0) {
        delete_selected_text();
        request_repaint();
    } else {
        if (cursor_pos > 0) {
            CL_UTF8_Reader utf8_reader(text);
            utf8_reader.set_position(cursor_pos);
            utf8_reader.prev();
            int length = utf8_reader.get_char_length();
            text.erase(cursor_pos-length, length);
            cursor_pos -= length;
            request_repaint();
        }
    }

    int old_pos = get_cursor_pos();
    set_cursor_pos(0);
    set_cursor_pos(old_pos);
}

void LineEdit::del() {
    if (undo_info.first_erase) {
        undo_info.first_erase = false;
        undo_info.undo_text = get_text();
    }

    if (get_selection_length() != 0) {
        delete_selected_text();
        request_repaint();
    } else {
        if (cursor_pos < (int)text.size()) {
            CL_UTF8_Reader utf8_reader(text);
            utf8_reader.set_position(cursor_pos);
            int length = utf8_reader.get_char_length();
            text.erase(cursor_pos,length);
            request_repaint();
        }
    }
}

int LineEdit::get_character_index(int mouse_x_wincoords) {
    if (text.size() <= 1 ) {
        return text.size();
    }

    CL_GraphicContext &gc = get_gc();
    CL_UTF8_Reader utf8_reader(text);

    int mouse_x = mouse_x_wincoords;

    int seek_start = clip_start_offset;
    int seek_end = text.size();
    int seek_center = (seek_start + seek_end) / 2;

    //fast search
    CL_Font font = getFont();
    while (true) {
        utf8_reader.set_position(seek_center);
        utf8_reader.move_to_leadbyte();

        seek_center = utf8_reader.get_position();

        CL_Size text_size = get_visual_text_size(gc, font, clip_start_offset, seek_center - clip_start_offset);

        if (text_size.width > mouse_x)
            seek_end = seek_center;
        else
            seek_start = seek_center;

        if(seek_end - seek_start < 7)
            break; //go to accurate search

        seek_center = (seek_start + seek_end) / 2;
    }

    utf8_reader.set_position(seek_start);
    utf8_reader.move_to_leadbyte();

    //accurate search
    while (true) {
        seek_center = utf8_reader.get_position();

        CL_Size text_size = get_visual_text_size(gc, font, clip_start_offset, seek_center - clip_start_offset);
        if (text_size.width > mouse_x || utf8_reader.is_end())
            break;

        utf8_reader.next();
    }

    return seek_center;
}

void LineEdit::update_text_clipping() {
    CL_GraphicContext &gc = get_gc();

    //CL_Size text_size = get_visual_text_size(gc, getFont(), clip_start_offset, text.size() - clip_start_offset);

    if (cursor_pos < clip_start_offset)
        clip_start_offset = cursor_pos;

    CL_Rect cursor_rect = get_cursor_rect();

    CL_UTF8_Reader utf8_reader(text);
    while (cursor_rect.right > get_width()) {
        utf8_reader.set_position(clip_start_offset);
        utf8_reader.next();
        clip_start_offset = utf8_reader.get_position();
        if (clip_start_offset == (int)text.size())
            break;
        cursor_rect = get_cursor_rect();
    }

    // Get number of chars of current text fitting in the lineedit.
    int search_upper = text.size();
    int search_lower = clip_start_offset;

    CL_Font font = getFont();
    while (true) {
        int midpoint = (search_lower + search_upper) / 2;

        utf8_reader.set_position(midpoint);
        utf8_reader.move_to_leadbyte();
        if (midpoint != (int)utf8_reader.get_position())
            utf8_reader.next();
        midpoint = utf8_reader.get_position();

        if (midpoint == search_lower || midpoint == search_upper)
            break;

        CL_Size midpoint_size = get_visual_text_size(gc, font, clip_start_offset, midpoint-clip_start_offset);

        if (get_width() < midpoint_size.width)
            search_upper = midpoint;
        else
            search_lower = midpoint;
    }
    clip_end_offset = search_upper;

    if (cursor_rect.left < 0) {
        clip_start_offset = cursor_pos;
    }
}

CL_Rect LineEdit::get_cursor_rect() {
    CL_GraphicContext &gc = get_gc();

    CL_Rect cursor_rect;

    int substr_end = cursor_pos - clip_start_offset;
    if (substr_end < 0)
        substr_end = 0;

    CL_String clipped_text = text.substr(clip_start_offset, substr_end);

    if ( password_mode ) {
        // If we are in password mode, we gonna return the right characters
        clipped_text = create_password(clipped_text.utf8_length());
    }

    CL_Size text_size_before_cursor = getFont().get_text_size(gc, clipped_text);


    // hack. uo fonts always add 2 for border size. border is not drawn when color is not black (gnarf!)
    if (isUoFont_) {
        // only parts of border required for the other two parts
        if (text_size_before_cursor.width > 0) {
            text_size_before_cursor.width -= 1;
        }
    }

    cursor_rect.left = text_size_before_cursor.width;
    cursor_rect.right = cursor_rect.left + 1;


    CL_Font font = getFont();
    CL_FontMetrics metrics = font.get_font_metrics();
    float align_height = metrics.get_ascent() - metrics.get_internal_leading();
    cursor_rect.top = ((get_height() - align_height) / 2.0f) - metrics.get_internal_leading();
    cursor_rect.bottom = cursor_rect.top + metrics.get_ascent() + metrics.get_descent();

    return cursor_rect;
}

CL_Rect LineEdit::get_selection_rect() {
    CL_GraphicContext &gc = get_gc();

    CL_Font font = getFont();
    // text before selection:
    CL_String txt_before = get_visible_text_before_selection();
    CL_Size text_size_before_selection = font.get_text_size(gc, txt_before);

    // selection text:
    CL_String txt_selected = get_visible_selected_text();
    CL_Size text_size_selection = font.get_text_size(gc, txt_selected);

    // hack. uo fonts always add 2 for border size. border is not drawn when color is not black (gnarf!)
    if (isUoFont_) {
        // no extra border required for selected part
        if (text_size_selection.width > 0) {
            text_size_selection.width -= 2;
        }
        // only parts of border required for the other two parts
        if (text_size_before_selection.width > 0) {
            text_size_before_selection.width -= 1;
        }
    }

    CL_Rect selection_rect;
    selection_rect.left = text_size_before_selection.width;
    selection_rect.right = selection_rect.left + text_size_selection.width;

    CL_FontMetrics metrics = getFont().get_font_metrics();
    float align_height = metrics.get_ascent() - metrics.get_internal_leading();
    selection_rect.top = ((get_height() - align_height) / 2.0f) - metrics.get_internal_leading();
    selection_rect.bottom = selection_rect.top + metrics.get_ascent() + metrics.get_descent();

    return selection_rect;
}

CL_String LineEdit::break_characters = " ::;,.-";

int LineEdit::find_next_break_character(int search_start) {
    if (search_start >= int(text.size())-1)
        return text.size();

    unsigned int pos = text.find_first_of(break_characters, search_start);
    if (pos == CL_String::npos)
        return text.size();
    return pos;
}

int LineEdit::find_previous_break_character(int search_start) {
    if (search_start <= 0)
        return 0;
    unsigned int pos = text.find_last_of(break_characters, search_start);
    if (pos == CL_String::npos)
        return 0;
    return pos;
}

void LineEdit::on_timer_expired() {
    if (is_visible() == false) {
        timer.stop();
        return;
    }

    if (cursor_blink_visible)
        timer.start(500);
    else
        timer.start(500);

    cursor_blink_visible = !cursor_blink_visible;
    request_repaint();
}

CL_String LineEdit::get_visible_text_before_selection() {
    CL_String ret;
    int sel_start = cl_min(selection_start, selection_start+selection_length);
    int start = cl_min(sel_start, clip_start_offset);

    if (start < clip_start_offset)
        return ret;

    int end = cl_min(sel_start, clip_end_offset);

    ret = text.substr(start, end - start);

    // If we are in password mode, we gonna return the right characters
    if ( password_mode )
        ret = create_password(ret.utf8_length());

    return ret;
}

CL_String LineEdit::get_visible_selected_text() {
    CL_String ret;

    if (selection_length == 0)
        return ret;

    int sel_start = cl_min(selection_start, selection_start+selection_length);
    int sel_end = cl_max(selection_start, selection_start + selection_length);
    int end = cl_min(clip_end_offset, sel_end);
    int start = cl_max(clip_start_offset, sel_start);

    if (start > end)
        return ret;

    if (start == end)
        return ret;

    ret = text.substr(start, end - start);

    // If we are in password mode, we gonna return the right characters
    if ( password_mode )
        ret = create_password(ret.utf8_length());

    return ret;
}

void LineEdit::set_selection_start(int start) {
    selection_start = start;
}

void LineEdit::set_selection_length(int length) {
    selection_length = length;
}

void LineEdit::set_text_selection(int start, int length) {
    selection_start = start;
    selection_length = length;
}

CL_String LineEdit::get_visible_text_after_selection() {
    // returns the whole visible string if there is no selection.
    CL_String ret;

    int sel_end = cl_max(selection_start, selection_start + selection_length);
    int start = cl_max(clip_start_offset, sel_end);

    int end = clip_end_offset;
    if (start > end)
        return ret;

    if (clip_end_offset == sel_end)
        return ret;

    if (sel_end <= 0)
        return ret;
    else {
        ret = text.substr(start, end - start);
        // If we are in password mode, we gonna return the right characters
        if ( password_mode )
            ret = create_password(ret.utf8_length());

        return ret;
    }
}

void LineEdit::on_render(CL_GraphicContext &gc, const CL_Rect &update_rect) {
    CL_String txt_before = get_visible_text_before_selection();
    CL_String txt_selected = get_visible_selected_text();
    CL_String txt_after = get_visible_text_after_selection();

    if (txt_before.empty() && txt_selected.empty() && txt_after.empty()) {
        txt_after = text.substr(clip_start_offset, clip_end_offset - clip_start_offset);

        // If we are in password mode, we gonna return the right characters
        if ( password_mode )
            txt_after = create_password(txt_after.utf8_length());
    }

    CL_Size size_before = getFont().get_text_size(gc, txt_before);
    CL_Size size_selected = getFont().get_text_size(gc, txt_selected);

    // hack. uo fonts always add 2 for border size. border is not drawn when color is not black (gnarf!)
    if (isUoFont_) {
        if (size_selected.width > 0) {
            size_selected.width -= 2;
        }
        if (size_before.width > 0) {
            size_before.width -= 2;
        }
    }

    CL_FontMetrics metrics = getFont().get_font_metrics();
    float align_height = metrics.get_ascent() - metrics.get_internal_leading();
    int y = align_height + ((get_height() - align_height) / 2.0f);


    // Draw text before selection
    if (!txt_before.empty()) {
        getFont().draw_text(gc, 0, y, txt_before, fontColor_);
        //CL_Rect text_rect = content_rect;
        //text_rect.top = g.top;
        //text_rect.bottom = g.bottom;
        //part_component.render_text(gc, txt_before, text_rect, update_rect);
    }
    if (!txt_selected.empty()) {
        // Draw selection box.
        CL_Rect selection_rect = get_selection_rect();
        CL_Draw::fill(gc, selection_rect, CL_Colorf::grey);
        //part_selection.render_box(gc, selection_rect, update_rect);

        getFont().draw_text(gc, size_before.width, y, txt_selected, fontColor_);

        //CL_Rect text_rect = content_rect;
        //text_rect.left += (size_before.width);
        //text_rect.top = g.top;
        //text_rect.bottom = g.bottom;
        //part_selection.render_text(gc, txt_selected, text_rect, update_rect);
    }
    if (!txt_after.empty()) {
        getFont().draw_text(gc, size_before.width + size_selected.width, y, txt_after, fontColor_);
        //CL_Rect text_rect = content_rect;
        //text_rect.left += (size_before.width + size_selected.width);
        //text_rect.top = g.top;
        //text_rect.bottom = g.bottom;
        //part_component.render_text(gc, txt_after, text_rect, update_rect);
    }

    //// draw cursor
    if (has_focus() || (get_focus_policy() == CL_GUIComponent::focus_parent && cursor_drawing_enabled_when_parent_focused)) {
        if (cursor_blink_visible) {
            CL_Rect cursor_rect = get_cursor_rect();
            CL_Draw::fill(gc, cursor_rect, fontColor_);
        }
    }
}

void LineEdit::on_scroll_timer_expired() {
    CL_GUIMessage_Input msg;
    CL_InputEvent event;
    msg.set_event(event);

    if (mouse_moves_left)
        move(-1,event);
    else
        move(1, event);
}

bool LineEdit::input_mask_accepts_input(int cursor_pos, const CL_StringRef &str) {
    return str.find_first_not_of(input_mask) == CL_StringData8::npos;
}

CL_String LineEdit::create_password(CL_String::size_type num_letters) const {
    return CL_String(num_letters, '*');
}

CL_Size LineEdit::get_visual_text_size(CL_GraphicContext &gc, CL_Font &font, int pos, int npos) const {
    return password_mode ? font.get_text_size(gc, create_password(text.substr(pos, npos).utf8_length())) : font.get_text_size(gc, text.substr(pos, npos));
}

CL_Size LineEdit::get_visual_text_size(CL_GraphicContext &gc, CL_Font &font) const {
    return password_mode ? font.get_text_size(gc, create_password(text.utf8_length())) : font.get_text_size(gc, text);
}

}
}
}
