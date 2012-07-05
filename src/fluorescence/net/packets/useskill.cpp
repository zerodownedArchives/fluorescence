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



#include "useskill.hpp"

namespace fluo {
namespace net {
namespace packets {

UseSkill::UseSkill(unsigned int skillId) : Packet(0x12), skillId_(skillId) {
}

bool UseSkill::write(int8_t* buf, unsigned int len, unsigned int& index) const {
    bool ret = true;

    ret &= writePacketId(buf, len, index);

    unsigned int sizeHelper = preparePacketSize(index);

    uint8_t commandType = 0x24; // this is a weird multi-purpose packet.
    ret &= PacketWriter::write(buf, len, index, commandType);

    // skill id is sent as a string
    UnicodeString skillIdString = StringConverter::fromNumber(skillId_);
    ret &= PacketWriter::writeUtf8Null(buf, len, index, skillIdString);

    ret &= writePacketSize(buf, len, index, sizeHelper);

    return ret;
}

}
}
}
