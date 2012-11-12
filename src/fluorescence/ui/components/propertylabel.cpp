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



#include "propertylabel.hpp"

#include <world/mobile.hpp>

#include <misc/log.hpp>

namespace fluo {
namespace ui {
namespace components {

PropertyLabel::PropertyLabel(CL_GUIComponent* parent, const UnicodeString& link) : Label(parent), linkName_(link) {
    set_type_name("propertylabel");
}

void PropertyLabel::update(const boost::shared_ptr<world::Mobile>& mob) {
    if (mob->hasProperty(linkName_)) {
        setText(mob->getProperty(linkName_).asString());
    }
}

}
}
}
