
#include "unicodetext.hpp"

namespace fluo {
namespace net {
namespace packets {

UnicodeText::UnicodeText() : BaseText(0xae) {
}

bool UnicodeText::read(const int8_t* buf, unsigned int len, unsigned int& index) {
    bool ret = true;

    ret = ret && PacketReader::read(buf, len, index, serial_);
    ret = ret && PacketReader::read(buf, len, index, bodyId_);
    ret = ret && PacketReader::read(buf, len, index, type_);
    ret = ret && PacketReader::read(buf, len, index, hue_);
    ret = ret && PacketReader::read(buf, len, index, font_);
    ret = ret && PacketReader::read(buf, len, index, language_[0]);
    ret = ret && PacketReader::read(buf, len, index, language_[1]);
    ret = ret && PacketReader::read(buf, len, index, language_[2]);
    ret = ret && PacketReader::read(buf, len, index, language_[3]);
    ret = ret && PacketReader::readUtf8Fixed(buf, len, index, speaker_, 30);
    ret = ret && PacketReader::readUnicodeNull(buf, len, index, text_);

    return ret;
}

}
}
}
