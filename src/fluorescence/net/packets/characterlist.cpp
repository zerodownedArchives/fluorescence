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



#include "characterlist.hpp"

#include <misc/log.hpp>
#include <ui/manager.hpp>
#include <ui/gumpmenus.hpp>

namespace fluo {
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
