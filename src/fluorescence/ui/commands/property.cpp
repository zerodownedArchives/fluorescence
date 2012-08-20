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

#include "property.hpp"

#include <typedefs.hpp>
#include <world/manager.hpp>
#include <world/mobile.hpp>

namespace fluo {
namespace ui {
namespace commands {

Property::Property() : ClientCommand("Usage: property <name>. Displays the given property of the player") {
}

void Property::execute(const UnicodeString& args) {
    UnicodeString msg("Property ");
    msg += args;
    if (!world::Manager::getSingleton()->getPlayer()->hasProperty(args)) {
        msg += " not found";
    } else {
        msg += ": ";
        msg += world::Manager::getSingleton()->getPlayer()->getProperty(args).asString();
    }
    world::Manager::getSingleton()->systemMessage(msg);
}

}
}
}

