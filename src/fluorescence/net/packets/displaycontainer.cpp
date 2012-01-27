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



#include "displaycontainer.hpp"

#include <boost/shared_ptr.hpp>

#include <world/manager.hpp>
#include <world/dynamicitem.hpp>

#include <misc/log.hpp>

namespace fluo {
namespace net {
namespace packets {

DisplayContainer::DisplayContainer() : Packet(0x24, 7) {
}

bool DisplayContainer::read(const int8_t* buf, unsigned int len, unsigned int& index) {
    bool ret = true;

    ret &= PacketReader::read(buf, len, index, serial_);
    ret &= PacketReader::read(buf, len, index, gumpId_);

    return ret;
}

void DisplayContainer::onReceive() {
    boost::shared_ptr<world::DynamicItem> itm = world::Manager::getSingleton()->getDynamicItem(serial_, false);
    if (itm) {
        itm->openContainerGump(gumpId_);
    } else {
        LOG_WARN << "Trying to open container with unknown serial " << serial_;
    }
}

}
}
}
