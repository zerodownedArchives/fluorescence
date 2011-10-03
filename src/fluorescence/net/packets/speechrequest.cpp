
#include "speechrequest.hpp"

namespace fluo {
namespace net {
namespace packets {

SpeechRequest::SpeechRequest(const UnicodeString& text) : Packet(0xAD), speechMode_(0), hue_(0), font_(0), text_(text) {
    language_[0] = 'e';
    language_[1] = 'n';
    language_[2] = 'u';
    language_[3] = '\0';
}

bool SpeechRequest::write(int8_t* buf, unsigned int len, unsigned int& index) const {
    bool ret = true;

    ret &= writePacketId(buf, len, index);

    unsigned int sizeHelper = preparePacketSize(index);

    ret &= PacketWriter::write(buf, len, index, speechMode_);
    ret &= PacketWriter::write(buf, len, index, hue_);
    ret &= PacketWriter::write(buf, len, index, font_);
    ret &= PacketWriter::write(buf, len, index, language_[0]);
    ret &= PacketWriter::write(buf, len, index, language_[1]);
    ret &= PacketWriter::write(buf, len, index, language_[2]);
    ret &= PacketWriter::write(buf, len, index, language_[3]);
    ret &= PacketWriter::writeUnicodeNull(buf, len, index, text_);

    writePacketSize(buf, len, index, sizeHelper);

    return ret;
}

}
}
}
