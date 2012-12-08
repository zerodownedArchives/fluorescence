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

#include "pyworld.hpp"

#include <net/manager.hpp>
#include <net/packets/06_doubleclick.hpp>
#include <net/packets/b8_profile.hpp>
#include <world/manager.hpp>
#include <world/mobile.hpp>
#include <world/dynamicitem.hpp>

namespace fluo {
namespace ui {
namespace python {

boost::shared_ptr<world::Mobile> PyWorld::getPlayer() {
    return world::Manager::getSingleton()->getPlayer();
}

boost::shared_ptr<world::Mobile> PyWorld::getMobile(Serial serial) {
    return world::Manager::getSingleton()->getMobile(serial, false);
}

boost::shared_ptr<world::DynamicItem> PyWorld::getDynamicItem(Serial serial) {
    return world::Manager::getSingleton()->getDynamicItem(serial, false);
}

void PyWorld::openProfile(const boost::shared_ptr<world::Mobile>& self) {
    net::packets::ProfileRequest pkt(self->getSerial());
    net::Manager::getSingleton()->send(pkt);
}

void PyWorld::openPaperdoll(const boost::shared_ptr<world::Mobile>& self) {
    net::packets::DoubleClick pkt(self->getSerial());
    net::Manager::getSingleton()->send(pkt);
}

}
}
}

