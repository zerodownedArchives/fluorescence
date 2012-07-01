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



#include "speechrequest.hpp"

namespace fluo {
namespace net {
namespace packets {

SpeechRequest::SpeechRequest(unsigned int speechMode, unsigned int hue, unsigned int font, const UnicodeString& text) : 
        Packet(0xAD), 
        speechMode_(speechMode), hue_(hue), font_(font), text_(text) {
	language_[0] = 'e';
	language_[1] = 'n';
	language_[2] = 'u';
	language_[2] = '\0';
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
