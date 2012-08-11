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

#include "warmodebutton.hpp"

#include <ClanLib/Display/Window/keys.h>

#include <misc/log.hpp>
#include <ui/gumpmenu.hpp>
#include <world/mobile.hpp>

namespace fluo {
namespace ui {
namespace components {

WarModeButton::WarModeButton(CL_GUIComponent* parent) : MultiTextureImage(parent, 6), mouseOver_(false), mouseDown_(false), warmode_(false) {
    func_input_pressed().set(this, &WarModeButton::onInputPressed);
    func_input_released().set(this, &WarModeButton::onInputReleased);
    func_pointer_enter().set(this, &WarModeButton::onPointerEnter);
    func_pointer_exit().set(this, &WarModeButton::onPointerExit);

    set_double_click_enabled(false);

    set_type_name("warmodebutton");
}

bool WarModeButton::onInputReleased(const CL_InputEvent & e) {
    if (e.id == CL_MOUSE_LEFT) {
        mouseDown_ = false;
        updateTexture();

        world::Mobile* linkedMobile = getTopLevelMenu()->getLinkedMobile();
        if (!linkedMobile) {
            LOG_ERROR << "WarModeButton in unlinked gump" << std::endl;
        } else {
            linkedMobile->setWarMode(!linkedMobile->isWarMode());
        }

        return true;
    } else {
        return false;
    }
}

bool WarModeButton::onInputPressed(const CL_InputEvent & e) {
    if (e.id == CL_MOUSE_LEFT) {
        mouseDown_ = true;
        updateTexture();
        return true;
    } else {
        return false;
    }
}

GumpMenu* WarModeButton::getTopLevelMenu() {
    CL_GUIComponent* topLevel = get_top_level_component();
    return dynamic_cast<GumpMenu*>(topLevel);
}

bool WarModeButton::onPointerEnter() {
    mouseOver_ = true;
    updateTexture();

    return true;
}

bool WarModeButton::onPointerExit() {
    mouseOver_ = false;
    mouseDown_ = false;
    updateTexture();

    return true;
}

void WarModeButton::updateTexture() {
    unsigned int idx = calcTextureId();
    activateTexture(idx);
}

unsigned int WarModeButton::calcTextureId() const {
    unsigned int wmOffset = warmode_ ? 3 : 0;
    if (mouseDown_) {
        return WarModeButton::TEX_INDEX_DOWN + wmOffset;
    } else {
        if (mouseOver_) {
            return WarModeButton::TEX_INDEX_MOUSEOVER + wmOffset;
        } else {
            return WarModeButton::TEX_INDEX_UP + wmOffset;
        }
    }
}

void WarModeButton::setWarMode(bool warmode) {
    if (warmode_ != warmode) {
        warmode_ = warmode;
        updateTexture();
    }
}

}
}
}
