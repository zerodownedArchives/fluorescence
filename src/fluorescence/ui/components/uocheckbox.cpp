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

#include "uocheckbox.hpp"

#include <ClanLib/Display/Window/keys.h>

#include <ui/gumpmenu.hpp>

namespace fluo {
namespace ui {
namespace components {

UoCheckbox::UoCheckbox(CL_GUIComponent* parent) : MultiTextureImage<4>(parent), mouseOver_(false), checked_(false), switchId_(0) {
    func_input_pressed().set(this, &UoCheckbox::onInputPressed);
    func_pointer_enter().set(this, &UoCheckbox::onPointerEnter);
    func_pointer_exit().set(this, &UoCheckbox::onPointerExit);
    
    set_double_click_enabled(false);
}

bool UoCheckbox::onInputPressed(const CL_InputEvent & e) {
    if (e.id == CL_MOUSE_LEFT) {
        setChecked(!checked_);
        return true;
    } else {
        return false;
    }
}

bool UoCheckbox::onPointerEnter() {
    mouseOver_ = true;
    updateTexture();
    
    return true;
}

bool UoCheckbox::onPointerExit() {
    mouseOver_ = false;
    updateTexture();
    
    return true;
}

void UoCheckbox::updateTexture() {
    unsigned int idx = calcTextureId();
    activateTexture(idx);
}

unsigned int UoCheckbox::calcTextureId() const {
    if (checked_) {
        if (mouseOver_) {
            return 3;
        } else {
            return 2;
        }
    } else {
        if (mouseOver_) {
            return 1;
        } else {
            return 0;
        }
    }
}

void UoCheckbox::setChecked(bool value) {
    checked_ = value;
    updateTexture();
}

bool UoCheckbox::isChecked() const {
    return checked_;
}

}
}
}
