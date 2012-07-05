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

#include "b1_gumpreply.hpp"

namespace fluo {
namespace net {
namespace packets {

GumpReply::GumpReply(Serial gumpSerial, unsigned int typeId, unsigned int buttonId,
                const std::list<uint32_t>& switches, const std::list<TextEntryInfo>& textEntries_) :
        Packet(0xb1),
        gumpSerial_(gumpSerial), typeId_(typeId), buttonId_(buttonId), switches_(switches), textEntries_(textEntries_) {
}

bool GumpReply::write(int8_t* buf, unsigned int len, unsigned int& index) const {
    bool ret = true;

    ret &= writePacketId(buf, len, index);

    unsigned int sizeHelper = preparePacketSize(index);

    ret &= PacketWriter::write(buf, len, index, gumpSerial_);
    ret &= PacketWriter::write(buf, len, index, typeId_);
    ret &= PacketWriter::write(buf, len, index, buttonId_);

    uint32_t switchCount = switches_.size();
    ret &= PacketWriter::write(buf, len, index, switchCount);

    std::list<uint32_t>::const_iterator iterSwitches = switches_.begin();
    std::list<uint32_t>::const_iterator endSwitches = switches_.end();

    for (; iterSwitches != endSwitches; ++iterSwitches) {
        ret &= PacketWriter::write(buf, len, index, *iterSwitches);
    }

    uint32_t textCount = textEntries_.size();
    ret &= PacketWriter::write(buf, len, index, textCount);

    std::list<TextEntryInfo>::const_iterator iter = textEntries_.begin();
    std::list<TextEntryInfo>::const_iterator end = textEntries_.end();

    for (; iter != end; ++iter) {
        ret &= PacketWriter::write(buf, len, index, iter->number_);
        uint16_t textLength = iter->text_.length();
        ret &= PacketWriter::write(buf, len, index, textLength);
        ret &= PacketWriter::writeUnicodeFixed(buf, len, index, iter->text_, iter->text_.length());
    }

    writePacketSize(buf, len, index, sizeHelper);

    return ret;
}

}
}
}
