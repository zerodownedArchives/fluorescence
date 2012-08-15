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



#include "c2_unicodepromptreply.hpp"

namespace fluo {
namespace net {
namespace packets {

UnicodePromptReply::UnicodePromptReply(Serial serial, const UnicodeString& reply) :
        Packet(0xc2),
        promptSerial_(serial), reply_(reply), response_(1) {
}

UnicodePromptReply::UnicodePromptReply(Serial serial) :
        Packet(0xc2),
        promptSerial_(serial), response_(0) {
}

bool UnicodePromptReply::write(int8_t* buf, unsigned int len, unsigned int& index) const {
    bool ret = true;

    ret &= writePacketId(buf, len, index);

    unsigned int sizeHelper = preparePacketSize(index);

    ret &= PacketWriter::write(buf, len, index, promptSerial_);
    ret &= PacketWriter::write(buf, len, index, promptSerial_);

    ret &= PacketWriter::write(buf, len, index, response_);
    uint32_t zero = 0;
    ret &= PacketWriter::write(buf, len, index, zero);

    if (response_ == 1) {
        ret &= PacketWriter::writeUnicodeLENull(buf, len, index, reply_);
    } else {
        uint16_t uniNull = 0;
        ret &= PacketWriter::write(buf, len, index, uniNull);
    }

    writePacketSize(buf, len, index, sizeHelper);

    return ret;
}

}
}
}
