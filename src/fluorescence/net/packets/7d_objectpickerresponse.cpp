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



#include "7d_objectpickerresponse.hpp"

namespace fluo {
namespace net {
namespace packets {

ObjectPickerResponse::ObjectPickerResponse(unsigned int serial, unsigned int menuId, unsigned int answerId, unsigned int artId, unsigned int hue) :
        Packet(0x7d, 13),
        serial_(serial), menuId_(menuId), answerId_(answerId), artId_(artId), hue_(hue) {
}

bool ObjectPickerResponse::write(int8_t* buf, unsigned int len, unsigned int& index) const {
    bool ret = true;

    ret &= writePacketId(buf, len, index);

    ret &= PacketWriter::write(buf, len, index, serial_);
    ret &= PacketWriter::write(buf, len, index, menuId_);
    ret &= PacketWriter::write(buf, len, index, answerId_);
    ret &= PacketWriter::write(buf, len, index, artId_);
    ret &= PacketWriter::write(buf, len, index, hue_);

    return ret;
}

}
}
}
