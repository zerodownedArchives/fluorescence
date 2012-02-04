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

#include "uobutton.hpp"

#include <ClanLib/Display/Window/keys.h>

#include <ui/gumpmenu.hpp>

namespace fluo {
namespace ui {
namespace components {

UoButton::UoButton(CL_GUIComponent* parent) : MultiTextureImage<3>(parent), mouseOver_(false), mouseDown_(false) {
    func_input_pressed().set(this, &UoButton::onInputPressed);
    func_input_released().set(this, &UoButton::onInputReleased);
    func_pointer_enter().set(this, &UoButton::onPointerEnter);
    func_pointer_exit().set(this, &UoButton::onPointerExit);
}

bool UoButton::onInputReleased(const CL_InputEvent & e) {
    if (e.id == CL_MOUSE_LEFT) {
        mouseDown_ = false;
        updateTexture();
        handleClick();
        return true;
    } else {
        return false;
    }
}

bool UoButton::onInputPressed(const CL_InputEvent & e) {
    if (e.id == CL_MOUSE_LEFT) {
        mouseDown_ = true;
        updateTexture();
        return true;
    } else {
        return false;
    }
}

GumpMenu* UoButton::getTopLevelMenu() {
    CL_GUIComponent* topLevel = get_top_level_component();
    return dynamic_cast<GumpMenu*>(topLevel);
}

bool UoButton::onPointerEnter() {
    mouseOver_ = true;
    updateTexture();
    
    return true;
}

bool UoButton::onPointerExit() {
    mouseOver_ = false;
    mouseDown_ = false;
    updateTexture();
    
    return true;
}

void UoButton::updateTexture() {
    unsigned int idx = calcTextureId();
    activateTexture(idx);
}

unsigned int UoButton::calcTextureId() const {
    if (mouseDown_) {
        return 2;
    } else {
        if (mouseOver_) {
            return 1;
        } else {
            return 0;
        }
    }
}

}
}
}
