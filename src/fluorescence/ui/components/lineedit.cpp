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



#include "lineedit.hpp"

#include <ui/gumpmenu.hpp>
#include <ui/gumpactions.hpp>

namespace fluo {
namespace ui {
namespace components {

LineEdit::LineEdit(CL_GUIComponent* parent) : CL_LineEdit(parent) {
}

UnicodeString LineEdit::getText() {
    return StringConverter::fromUtf8(get_text());
}

void LineEdit::setText(const UnicodeString& text) {
    set_text(StringConverter::toUtf8String(text));
}

void LineEdit::setAction(const UnicodeString& action) {
    action_ = action;
    func_enter_pressed().set(this, &LineEdit::onEnterPressed);
}

void LineEdit::onEnterPressed() {
    if (action_.length() > 0) {
        GumpMenu* gump = dynamic_cast<GumpMenu*>(get_top_level_component());
        if (gump) {
            GumpActions::doAction(gump, action_, 0, NULL);
        }
    }
}

}
}
}
