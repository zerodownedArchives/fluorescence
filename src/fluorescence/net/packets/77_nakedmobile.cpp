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



#include "77_nakedmobile.hpp"

#include <world/manager.hpp>
#include <world/mobile.hpp>

#include <misc/log.hpp>

#include <ui/manager.hpp>

namespace fluo {
namespace net {
namespace packets {

NakedMobile::NakedMobile() : Packet(0x77, 17) {
}

bool NakedMobile::read(const int8_t* buf, unsigned int len, unsigned int& index) {
    bool ret = true;

    ret = ret && PacketReader::read(buf, len, index, serial_);
    ret = ret && PacketReader::read(buf, len, index, bodyId_);
    ret = ret && PacketReader::read(buf, len, index, locX_);
    ret = ret && PacketReader::read(buf, len, index, locY_);
    ret = ret && PacketReader::read(buf, len, index, locZ_);
    ret = ret && PacketReader::read(buf, len, index, direction_);
    ret = ret && PacketReader::read(buf, len, index, hue_);
    ret = ret && PacketReader::read(buf, len, index, status_);
    ret = ret && PacketReader::read(buf, len, index, notoriety_);

    return ret;
}

void NakedMobile::onReceive() {
    boost::shared_ptr<world::Mobile> mob = world::Manager::getSingleton()->getMobile(serial_);

    mob->setLocation(locX_, locY_, locZ_);
    mob->setBodyId(bodyId_);
    mob->setDirection(direction_);
    mob->setHue(hue_);

    mob->setStatusFlags(status_);
}

}
}
}
