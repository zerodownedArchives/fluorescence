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



#include "1b_playerinit.hpp"

#include <world/manager.hpp>
#include <world/mobile.hpp>

#include <ui/manager.hpp>

namespace fluo {
namespace net {
namespace packets {

PlayerInit::PlayerInit() : Packet(0x1b, 37) {
}

bool PlayerInit::read(const int8_t* buf, unsigned int len, unsigned int& index) {
    bool ret = true;

    ret &= PacketReader::read(buf, len, index, serial_);
    index += 4;
    ret &= PacketReader::read(buf, len, index, bodyId_);
    ret &= PacketReader::read(buf, len, index, locX_);
    ret &= PacketReader::read(buf, len, index, locY_);
    index += 1;
    ret &= PacketReader::read(buf, len, index, locZ_);
    ret &= PacketReader::read(buf, len, index, direction_);
    index += 19;

    return ret;
}

void PlayerInit::onReceive() {
    boost::shared_ptr<world::Mobile> player = world::Manager::getSingleton()->initPlayer(serial_);

    player->setLocation(locX_, locY_, locZ_);
    player->setBodyId(bodyId_);
    player->setDirection(direction_);
}

}
}
}
