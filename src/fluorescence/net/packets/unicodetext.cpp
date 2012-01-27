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
