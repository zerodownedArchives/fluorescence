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



#include "statupdatehitpoints.hpp"

#include <world/manager.hpp>
#include <world/mobile.hpp>

#include <misc/log.hpp>

namespace fluo {
namespace net {
namespace packets {

StatUpdateHitpoints::StatUpdateHitpoints() : Packet(0xA1, 9) {
}

bool StatUpdateHitpoints::read(const int8_t* buf, unsigned int len, unsigned int& index) {
    bool ret = true;

    ret &= PacketReader::read(buf, len, index, serial_);
    ret &= PacketReader::read(buf, len, index, maximum_);
    ret &= PacketReader::read(buf, len, index, current_);

    return ret;
}

void StatUpdateHitpoints::onReceive() {
    boost::shared_ptr<world::Mobile> mob = world::Manager::getSingleton()->getMobile(serial_, false);
    if (mob) {
        mob->getProperty("hitpoints").setInt(current_);
        mob->getProperty("hitpoints-max").setInt(maximum_);

        mob->onPropertyUpdate();
    } else {
        LOG_WARN << "Received full stat update for unknown mobile serial=" << serial_ << std::endl;
    }
}

}
}
}
