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

#include "speechentry.hpp"

#include <typedefs.hpp>
#include <ui/manager.hpp>
#include <ui/gumpmenu.hpp>
#include <misc/log.hpp>

namespace fluo {
namespace ui {
namespace commands {

void SpeechEntry::execute(const UnicodeString& args) {
    GumpMenu* gameWindow = ui::Manager::getSingleton()->getGumpMenu("gamewindow");
    if (gameWindow) {
        gameWindow->activatePage(2);

        CL_GUIComponent* lineedit = gameWindow->get_named_item("speechtext");
        if (lineedit) {
            lineedit->set_focus();
        } else {
            LOG_ERROR << "Unable to find speech lineedit in gamewindow" << std::endl;
        }
    } else {
        LOG_ERROR << "Unable to find gamewindow gump to activate speech lineedit" << std::endl;
    }
}

}
}
}

