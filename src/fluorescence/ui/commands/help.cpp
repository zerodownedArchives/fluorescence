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

#include "help.hpp"

#include <ui/manager.hpp>
#include <ui/commandmanager.hpp>
#include <world/manager.hpp>

namespace fluo {
namespace ui {
namespace commands {

Help::Help() : ClientCommand("Usage: help [command]. Display a list of all commands, or details about one command") {
}

void Help::execute(const UnicodeString& args) {
    if (args.length() == 0) {
        // list all commands
        UnicodeString list = ui::Manager::getCommandManager()->getCommandList();
        world::Manager::getSingleton()->systemMessage(list);
    } else {
        UnicodeString hlp = ui::Manager::getCommandManager()->getHelpText(args);
        world::Manager::getSingleton()->systemMessage(hlp);
    }
}

}
}
}

