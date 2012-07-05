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



#include "72_warmode.hpp"

#include <world/manager.hpp>
#include <world/mobile.hpp>

namespace fluo {
namespace net {
namespace packets {

WarMode::WarMode() : Packet(0x72, 5) {
}

WarMode::WarMode(bool warMode) : Packet(0x72, 5), warMode_(warMode ? 1 : 0) {
}

bool WarMode::write(int8_t* buf, unsigned int len, unsigned int& index) const {
    bool ret = true;

    ret &= writePacketId(buf, len, index);
    ret &= PacketWriter::write(buf, len, index, warMode_);

    uint8_t unknown = 0;
    ret &= PacketWriter::write(buf, len, index, unknown);
    unknown = 32;
    ret &= PacketWriter::write(buf, len, index, unknown);
    unknown = 0;
    ret &= PacketWriter::write(buf, len, index, unknown);

    return ret;
}

bool WarMode::read(const int8_t* buf, unsigned int len, unsigned int& index) {
    bool ret = PacketReader::read(buf, len, index, warMode_);

    // jump 3 unknown bytes
    index += 3;

    return ret;
}

void WarMode::onReceive() {
    world::Manager::getSingleton()->getPlayer()->setWarMode(warMode_ == 1);
}

}
}
}
