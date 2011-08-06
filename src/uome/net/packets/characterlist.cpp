
#include "characterlist.hpp"

#include <misc/log.hpp>
#include <ui/manager.hpp>
#include <ui/gumpmenus.hpp>

namespace uome {
namespace net {
namespace packets {

CharacterList::CharacterList() : Packet(0xa9), flags_(0) {
}

bool CharacterList::read(const int8_t* buf, unsigned int len, unsigned int& index) {
    bool ret = true;

    ret &= PacketReader::read(buf, len, index, charCount_);
    for (unsigned int i = 0; i < charCount_; ++i) {
        ret &= PacketReader::readUtf8Fixed(buf, len, index, charNames_[i], 30);
        ret &= PacketReader::readUtf8Fixed(buf, len, index, charPasswords_[i], 30);
    }

    ret &= PacketReader::read(buf, len, index, cityCount_);

    uint8_t idx;
    UnicodeString tmp;
    LOG_DEBUG << "city count: " << (unsigned int)cityCount_ << std::endl;
    for (unsigned int i = 0; i < cityCount_; ++i) {
        LOG_DEBUG << "reading city: " << i << std::endl;
        ret &= PacketReader::read(buf, len, index, idx);
        cityIndices_.push_back(idx);

        LOG_DEBUG << "index: " << idx << std::endl;

        ret &= PacketReader::readUtf8Fixed(buf, len, index, tmp, 31);
        cityNames_.push_back(tmp);

        LOG_DEBUG << "city name: " << tmp << std::endl;

        ret &= PacketReader::readUtf8Fixed(buf, len, index, tmp, 31);
        tavernNames_.push_back(tmp);

        LOG_DEBUG << "tavern name: " << tmp << std::endl;
    }

    unsigned int readSize = 5 + charCount_*60 + cityCount_*63;
    if (size_ - readSize == 4) {
        PacketReader::read(buf, len, index, flags_);
    }

    // prevent empty char slots from being displayed
    for (int i = charCount_-1; i >= 0; --i) {
        if (charNames_[i].isBogus() || charNames_[i].isEmpty()) {// || charNames_[i].getBuffer()[0] == 0) {
            charCount_--;
        }
    }

    return ret;
}

void CharacterList::onReceive() {
    ui::Manager::getSingleton()->closeGumpMenu("serverlist");
    ui::GumpMenus::openCharacterListGump(this);
}

}
}
}
