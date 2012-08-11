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

#include "checkbox.hpp"

#include <ClanLib/Display/Window/keys.h>

#include <ui/gumpmenu.hpp>

namespace fluo {
namespace ui {
namespace components {

Checkbox::Checkbox(CL_GUIComponent* parent) : MultiTextureImage(parent, 4), mouseOver_(false), checked_(false), switchId_(0) {
    func_input_pressed().set(this, &Checkbox::onInputPressed);
    func_pointer_enter().set(this, &Checkbox::onPointerEnter);
    func_pointer_exit().set(this, &Checkbox::onPointerExit);

    set_double_click_enabled(false);

    set_type_name("checkbox");
}

bool Checkbox::onInputPressed(const CL_InputEvent & e) {
    if (e.id == CL_MOUSE_LEFT) {
        setChecked(!checked_);
        return true;
    } else {
        return false;
    }
}

bool Checkbox::onPointerEnter() {
    mouseOver_ = true;
    updateTexture();

    return true;
}

bool Checkbox::onPointerExit() {
    mouseOver_ = false;
    updateTexture();

    return true;
}

void Checkbox::updateTexture() {
    unsigned int idx = calcTextureId();
    activateTexture(idx);
}

unsigned int Checkbox::calcTextureId() const {
    if (checked_) {
        if (mouseOver_) {
            return TEX_INDEX_CHECKED_MOUSEOVER;
        } else {
            return TEX_INDEX_CHECKED;
        }
    } else {
        if (mouseOver_) {
            return TEX_INDEX_UNCHECKED_MOUSEOVER;
        } else {
            return TEX_INDEX_UNCHECKED;
        }
    }
}

void Checkbox::setChecked(bool value) {
    checked_ = value;
    updateTexture();
}

bool Checkbox::isChecked() const {
    return checked_;
}

void Checkbox::setSwitchId(unsigned int id) {
    switchId_ = id;
}

unsigned int Checkbox::getSwitchId() const {
    return switchId_;
}

}
}
}
