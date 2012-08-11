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



#include "clicklabel.hpp"

#include <ClanLib/Display/Window/keys.h>

#include <misc/log.hpp>
#include <ui/gumpmenu.hpp>

namespace fluo {
namespace ui {
namespace components {

ClickLabel::ClickLabel(CL_GUIComponent* parent) : Label(parent) {
    func_input_released().set(this, &ClickLabel::onInputReleased);
    func_input_pressed().set(this, &ClickLabel::onInputPressed);

    set_type_name("clicklabel");
}

bool ClickLabel::onInputPressed(const CL_InputEvent & e) {
    // without this, only double clicks work
    return true;
}

bool ClickLabel::onInputReleased(const CL_InputEvent & e) {
    if (e.id == CL_MOUSE_LEFT) {
        handleClick();
        return true;
    } else {
        return false;
    }
}

GumpMenu* ClickLabel::getTopLevelMenu() {
    CL_GUIComponent* topLevel = get_top_level_component();
    return dynamic_cast<GumpMenu*>(topLevel);
}

}
}
}
