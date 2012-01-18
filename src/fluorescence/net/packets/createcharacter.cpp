
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
