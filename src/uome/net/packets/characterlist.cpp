
#include "characterlist.hpp"

#include <misc/log.hpp>

namespace uome {
namespace net {
namespace packets {

CharacterList::CharacterList() : Packet(0xa9), flags_(0) {
}

bool CharacterList::read(const int8_t* buf, unsigned int len, unsigned int& index) {
    bool ret = true;

    ret &= PacketReader::read(buf, len, index, charCount_);
    LOG_DEBUG << "Char count: " << (unsigned int)charCount_ << std::endl;
    for (unsigned int i = 0; i < charCount_; ++i) {
        ret &= PacketReader::readUtf8Fixed(buf, len, index, charNames_[i], 30);
        ret &= PacketReader::readUtf8Fixed(buf, len, index, charPasswords_[i], 30);
    }

    ret &= PacketReader::read(buf, len, index, cityCount_);
    LOG_DEBUG << "City count: " << (unsigned int)cityCount_ << std::endl;

    uint8_t idx;
    UnicodeString tmp;
    for (unsigned int i = 0; i < cityCount_; ++i) {
        ret &= PacketReader::read(buf, len, index, idx);
        cityIndices_.push_back(idx);

        ret &= PacketReader::readUtf8Fixed(buf, len, index, tmp, 31);
        cityNames_.push_back(tmp);

        ret &= PacketReader::readUtf8Fixed(buf, len, index, tmp, 31);
        tavernNames_.push_back(tmp);
    }

    unsigned int readSize = 5 + charCount_*60 + cityCount_*63;
    if (size_ - readSize == 4) {
        PacketReader::read(buf, len, index, flags_);
    }

    return ret;
}

void CharacterList::onReceive() {
}

}
}
}
