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



#include "movementaccept.hpp"

#include <net/manager.hpp>
#include <net/walkpacketmanager.hpp>

namespace fluo {
namespace net {
namespace packets {

MovementAccept::MovementAccept() : Packet(0x22, 3), sequence_(0), status_(0) {
}

bool MovementAccept::write(int8_t* buf, unsigned int len, unsigned int& index) const {
    bool ret = true;

    ret &= writePacketId(buf, len, index);
    ret &= PacketWriter::write(buf, len, index, sequence_);
    ret &= PacketWriter::write(buf, len, index, status_);

    return ret;
}

bool MovementAccept::read(const int8_t* buf, unsigned int len, unsigned int& index) {
    bool ret = true;

    ret &= PacketReader::read(buf, len, index, sequence_);
    ret &= PacketReader::read(buf, len, index, status_);

    return ret;
}

void MovementAccept::onReceive() {
    net::Manager::getWalkPacketManager()->onMovementAccept(sequence_);

    // TODO: handle status
}

}
}
}
