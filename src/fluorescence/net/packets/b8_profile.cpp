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



#include "b8_profile.hpp"

#include <misc/log.hpp>
#include <world/manager.hpp>
#include <world/mobile.hpp>

namespace fluo {
namespace net {
namespace packets {

ProfileRequest::ProfileRequest(Serial serial) : Packet(0xB8), serial_(serial) {
}

bool ProfileRequest::write(int8_t* buf, unsigned int len, unsigned int& index) const {
    bool ret = true;

    ret &= writePacketId(buf, len, index);
    uint16_t packetSize = 8;
    ret &= PacketWriter::write(buf, len, index, packetSize);
    uint8_t mode = 0;
    ret &= PacketWriter::write(buf, len, index, mode);
    ret &= PacketWriter::write(buf, len, index, serial_);

    return ret;
}


ProfileChangeRequest::ProfileChangeRequest(Serial serial, const UnicodeString& text) : Packet(0xB8), serial_(serial), text_(text) {
}

bool ProfileChangeRequest::write(int8_t* buf, unsigned int len, unsigned int& index) const {
    bool ret = true;

    ret &= writePacketId(buf, len, index);
    unsigned int sizeHelper = preparePacketSize(index);

    uint8_t mode = 1;
    ret &= PacketWriter::write(buf, len, index, mode);
    ret &= PacketWriter::write(buf, len, index, serial_);

    uint16_t strLen = text_.length();
    ret &= PacketWriter::write(buf, len, index, strLen);
    ret &= PacketWriter::writeUnicodeFixed(buf, len, index, text_, strLen);

    writePacketSize(buf, len, index, sizeHelper);
    return ret;
}


ProfileResponse::ProfileResponse() : Packet(0xB8) {
}

bool ProfileResponse::read(const int8_t* buf, unsigned int len, unsigned int& index) {
    bool ret = true;

    ret &= PacketReader::read(buf, len, index, serial_);
    ret &= PacketReader::readUtf8Null(buf, len, index, header_);
    ret &= PacketReader::readUnicodeNull(buf, len, index, nonEditText_);
    ret &= PacketReader::readUnicodeNull(buf, len, index, editText_);

    return ret;
}

void ProfileResponse::onReceive() {
    boost::shared_ptr<world::Mobile> mob = world::Manager::getSingleton()->getMobile(serial_, false);
    if (mob) {
        mob->getProperty("profile-header").setString(header_);
        mob->getProperty("profile-text").setString(editText_);
        mob->getProperty("profile-static").setString(nonEditText_);

        mob->onPropertyUpdate();
        mob->openProfile();
    } else {
        LOG_WARN << "Received profile for unknown mobile serial=" << serial_ << std::endl;
    }
}


}
}
}
