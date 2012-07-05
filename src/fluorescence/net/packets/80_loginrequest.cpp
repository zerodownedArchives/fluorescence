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



#include "80_loginrequest.hpp"

namespace fluo {
namespace net {
namespace packets {

LoginRequest::LoginRequest(const UnicodeString& name, const UnicodeString& password) : Packet(0x80, 62),
    name_(name), password_(password) {
}

bool LoginRequest::write(int8_t* buf, unsigned int len, unsigned int& index) const {
    bool ret = true;

    ret = ret && writePacketId(buf, len, index);
    ret = ret && PacketWriter::writeUtf8Fixed(buf, len, index, name_, 30);
    ret = ret && PacketWriter::writeUtf8Fixed(buf, len, index, password_, 30);
    ret = ret && PacketWriter::write(buf, len, index, (uint8_t)0);

    return ret;
}

}
}
}
