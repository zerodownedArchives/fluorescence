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

#include "uoradiobutton.hpp"

#include <ClanLib/Display/Window/keys.h>

namespace fluo {
namespace ui {
namespace components {

UoRadioButton::UoRadioButton(CL_GUIComponent* parent) : UoCheckbox(parent) {
    func_input_pressed().set(this, &UoRadioButton::onInputPressed);
}

void UoRadioButton::setRadioGroupId(unsigned int groupId) {
    radioGroupId_ = groupId;
}

unsigned int UoRadioButton::getRadioGroupId() const {
    return radioGroupId_;
}

bool UoRadioButton::onInputPressed(const CL_InputEvent & e) {
    if (e.id == CL_MOUSE_LEFT) {
        setChecked(true);
        
        std::vector<CL_GUIComponent*> children = get_top_level_component()->get_child_components();
        std::vector<CL_GUIComponent*>::const_iterator iter = children.begin();
        std::vector<CL_GUIComponent*>::const_iterator end = children.end();
        for (; iter != end; ++iter) {
            components::UoRadioButton* cur = dynamic_cast<components::UoRadioButton*>(*iter);
            if (cur && cur != this && cur->getRadioGroupId() == radioGroupId_ && cur->isChecked()) {
                cur->setChecked(false);
            }
        }
        
        return true;
    } else {
        return false;
    }
}

}
}
}
