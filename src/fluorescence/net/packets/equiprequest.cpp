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



#include "equiprequest.hpp"

namespace fluo {
namespace net {
namespace packets {

EquipRequest::EquipRequest(Serial item, Serial mobile) : Packet(0x13, 10), itemSerial_(item), mobileSerial_(mobile) {
}

bool EquipRequest::write(int8_t* buf, unsigned int len, unsigned int& index) const {
    bool ret = true;

    ret &= writePacketId(buf, len, index);

    ret &= PacketWriter::write(buf, len, index, itemSerial_);
    uint8_t layerDummy = 0;
    ret &= PacketWriter::write(buf, len, index, layerDummy);
    ret &= PacketWriter::write(buf, len, index, mobileSerial_);

    return ret;
}

}
}
}
