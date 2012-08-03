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



#ifndef FLUO_UI_COMPONENTS_SCROLLBAR_HPP
#define FLUO_UI_COMPONENTS_SCROLLBAR_HPP

#include <boost/shared_ptr.hpp>

#include <ClanLib/Core/System/timer.h>
#include <ClanLib/GUI/gui_message.h>
#include <ClanLib/GUI/gui_message_input.h>
#include <ClanLib/GUI/gui_theme_part.h>
#include <ClanLib/Display/Window/input_event.h>
#include <ClanLib/Display/Render/graphic_context.h>
#include <ClanLib/Core/Signals/callback_v0.h>

#include <ui/gumpcomponent.hpp>
#include <ui/texture.hpp>

namespace fluo {
namespace ui {

class XmlLoader;

namespace components {

class ScrollBar : public GumpComponent {

friend class fluo::ui::XmlLoader;

public:
    enum TextureIndex {
        TEX_INDEX_UP = 0,
        TEX_INDEX_MOUSEOVER = 1,
        TEX_INDEX_DOWN = 2,
    };

    enum Visibility {
        VISIBLE_ON_DEMAND = 0,
        VISIBLE_ALWAYS = 1,
        VISIBLE_NEVER = 2,
    };

    ScrollBar(CL_GUIComponent* parent);
    ~ScrollBar();

    void setTrackTexture(boost::shared_ptr<ui::Texture> texture, unsigned int hue = 0, const std::string& rgba = "");

    void setVisibility(unsigned int vis);
    unsigned int getVisibility() const;


    // clanlib stuff
    bool is_vertical() const;
	bool is_horizontal() const;
	int get_min() const;
	int get_max() const;
	int get_line_step() const;
	int get_page_step() const;
	int get_position() const;
	int get_preferred_width() const;
	int get_preferred_height() const;

	void set_vertical();
	void set_horizontal();
	void set_min(int scroll_min);
	void set_max(int scroll_max);
	void set_line_step(int step);
	void set_page_step(int step);
	void set_ranges(int min, int max, int lstep, int pstep);
	void calculate_ranges(int view_size, int total_size);
	void set_position(int pos);

    CL_Callback_v0 &func_scroll();

private:
    unsigned int incrementIndex_;
    boost::shared_ptr<ui::Texture> incrementTextures_[3];
    CL_Colorf incrementColors_[3];
    CL_Vec3f incrementHueInfos_[3];

    unsigned int decrementIndex_;
    boost::shared_ptr<ui::Texture> decrementTextures_[3];
    CL_Colorf decrementColors_[3];
    CL_Vec3f decrementHueInfos_[3];

    unsigned int thumbIndex_;
    boost::shared_ptr<ui::Texture> thumbTextures_[3];
    CL_Colorf thumbColors_[3];
    CL_Vec3f thumbHueInfos_[3];

    boost::shared_ptr<ui::Texture> trackTexture_;
    CL_Colorf trackColor_;
    CL_Vec3f trackHueInfo_;

    void updateTextureIndices(const CL_Point& pos);

    unsigned int visibility_;



    // clanlib stuff
    bool vertical;
	int scroll_min, scroll_max, line_step, page_step, position;

	enum MouseDownMode
	{
		mouse_down_none,
		mouse_down_button_decr,
		mouse_down_button_incr,
		mouse_down_track_decr,
		mouse_down_track_incr,
		mouse_down_thumb_drag
	} mouse_down_mode;

	int thumb_start_position;
	CL_Point mouse_drag_start_pos;
	int thumb_start_pixel_position;

	CL_Timer mouse_down_timer;
	int last_step_size;

	CL_Rect rect_button_decrement;
	CL_Rect rect_track_decrement;
	CL_Rect rect_thumb;
	CL_Rect rect_track_increment;
	CL_Rect rect_button_increment;

    void invoke_scroll_event();

    void on_process_message(CL_GUIMessage &msg);
	void on_mouse_move(CL_GUIMessage_Input &input, CL_InputEvent &input_event);
	void on_mouse_lbutton_down(CL_GUIMessage_Input &input, CL_InputEvent &input_event);
	void on_mouse_lbutton_up(CL_GUIMessage_Input &input, CL_InputEvent &input_event);
	void on_mouse_leave();
	void on_render(CL_GraphicContext &gc, const CL_Rect &update_rect);
	void on_enablemode_changed();
	void on_timer_expired();

	bool update_part_positions();
	int calculate_thumb_size(int track_size);
	int calculate_thumb_position(int thumb_size, int track_size);
	CL_Rect create_rect(const CL_Rect content_rect, int start, int end);

    CL_Callback_v0 func_scroll_;
};

}
}
}

#endif

