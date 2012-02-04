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



#include "templatebutton.hpp"

#include <ui/gumpmenu.hpp>

namespace fluo {
namespace ui {
namespace components {

TemplateButton::TemplateButton(CL_GUIComponent* parent) : CL_PushButton(parent) {
    func_clicked().set(this, &TemplateButton::onClick);
}

UnicodeString TemplateButton::getText() const {
    return StringConverter::fromUtf8(get_text());
}

void TemplateButton::setText(const UnicodeString& text) {
    set_text(StringConverter::toUtf8String(text));
}

void TemplateButton::onClick() {
    handleClick();
}

GumpMenu* TemplateButton::getTopLevelMenu() {
    CL_GUIComponent* topLevel = get_top_level_component();
    return dynamic_cast<GumpMenu*>(topLevel);
}

}
}
}
