
#include "asciitext.hpp"


#include <misc/log.hpp>

namespace uome {
namespace net {
namespace packets {

AsciiText::AsciiText() : Packet(0x1c) {
}

bool AsciiText::read(const int8_t* buf, unsigned int len, unsigned int& index) {
    bool ret = true;

    ret = ret && PacketReader::read(buf, len, index, serial_);
    ret = ret && PacketReader::read(buf, len, index, bodyId_);
    ret = ret && PacketReader::read(buf, len, index, type_);
    ret = ret && PacketReader::read(buf, len, index, hue_);
    ret = ret && PacketReader::read(buf, len, index, font_);
    ret = ret && PacketReader::readUtf8Fixed(buf, len, index, speaker_, 30);
    ret = ret && PacketReader::readUtf8Null(buf, len, index, text_);

    return ret;
}

void AsciiText::onReceive() {
    LOG_INFO << "Ascii msg from " << speaker_ << ": " << text_ << std::endl;
}

}
}
}
