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



#include "serverbutton.hpp"

#include <ui/manager.hpp>
#include <ui/gumpmenu.hpp>

namespace fluo {
namespace ui {
namespace components {

ServerButton::ServerButton(CL_GUIComponent* parent, unsigned int buttonId) : BaseButton(parent), buttonId_(buttonId) {
}

unsigned int ServerButton::getButtonId() {
    return buttonId_;
}

void ServerButton::onClicked(BaseButton* self) {
    GumpMenu* gump = dynamic_cast<GumpMenu*>(get_top_level_component());
    if (gump) {
        if (buttonId_ == 0) {
            ui::Manager::getSingleton()->closeGumpMenu(gump);
        } else {
            // TODO: send gump reply to server
        }
    }
}

}
}
}
