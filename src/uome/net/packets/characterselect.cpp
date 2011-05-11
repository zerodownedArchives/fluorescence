
#include "characterselect.hpp"

namespace uome {
namespace net {
namespace packets {

CharacterSelect::CharacterSelect(const UnicodeString& name, const UnicodeString& password, uint32_t index, uint32_t seed) : Packet(0x5d, 73),
    charName_(name), charPassword_(password), charIndex_(index), seed_(seed) {
}

bool CharacterSelect::write(int8_t* buf, unsigned int len, unsigned int& index) const {
    bool ret = true;

    ret &= writePacketInfo(buf, len, index);

    uint32_t unknown = 0xEDEDEDEDu;
    ret &= PacketWriter::write(buf, len, index, unknown);
    ret &= PacketWriter::writeUtf8Fixed(buf, len, index, charName_, 30);
    ret &= PacketWriter::writeUtf8Fixed(buf, len, index, charPassword_, 30);
    ret &= PacketWriter::write(buf, len, index, charIndex_);
    ret &= PacketWriter::write(buf, len, index, seed_);

    return ret;
}

}
}
}
