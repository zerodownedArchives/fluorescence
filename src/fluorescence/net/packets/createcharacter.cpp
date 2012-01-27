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



#include "createcharacter.hpp"

namespace fluo {
namespace net {
namespace packets {

CreateCharacter::CreateCharacter() : Packet(0x00, 104) {
}

bool CreateCharacter::write(int8_t* buf, unsigned int len, unsigned int& index) const {
    bool ret = true;

    ret &= writePacketId(buf, len, index);
    
    uint32_t dummy1 = 0xEDEDEDED;
    ret &= PacketWriter::write(buf, len, index, dummy1);
    
    uint32_t dummy2 = 0xFFFFFFFF;
    ret &= PacketWriter::write(buf, len, index, dummy2);
    
    uint8_t dummy3 = 0;
    ret &= PacketWriter::write(buf, len, index, dummy3);
    
    ret &= PacketWriter::writeUtf8Fixed(buf, len, index, name_, 30);
    ret &= PacketWriter::writeUtf8Fixed(buf, len, index, password_, 30);
    
    uint8_t gender = female_ ? 1 : 0;
    ret &= PacketWriter::write(buf, len, index, gender);
    
    ret &= PacketWriter::write(buf, len, index, strength_);
    ret &= PacketWriter::write(buf, len, index, dexterity_);
    ret &= PacketWriter::write(buf, len, index, intelligence_);
    ret &= PacketWriter::write(buf, len, index, skill1Id_);
    ret &= PacketWriter::write(buf, len, index, skill1Value_);
    ret &= PacketWriter::write(buf, len, index, skill2Id_);
    ret &= PacketWriter::write(buf, len, index, skill2Value_);
    ret &= PacketWriter::write(buf, len, index, skill3Id_);
    ret &= PacketWriter::write(buf, len, index, skill3Value_);
    ret &= PacketWriter::write(buf, len, index, hue_);
    ret &= PacketWriter::write(buf, len, index, hairStyle_);
    ret &= PacketWriter::write(buf, len, index, hairHue_);
    ret &= PacketWriter::write(buf, len, index, beardStyle_);
    ret &= PacketWriter::write(buf, len, index, beardHue_);
    ret &= PacketWriter::write(buf, len, index, startCity_);
    
    uint16_t dummy4 = 0;
    ret &= PacketWriter::write(buf, len, index, dummy4);
    
    ret &= PacketWriter::write(buf, len, index, slot_);
    ret &= PacketWriter::write(buf, len, index, encryptionKey_);
    ret &= PacketWriter::write(buf, len, index, shirtHue_);
    ret &= PacketWriter::write(buf, len, index, pantsHue_);
    
    return ret;
}

}
}
}
