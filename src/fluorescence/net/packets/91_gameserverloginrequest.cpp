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



#include "91_gameserverloginrequest.hpp"

namespace fluo {
namespace net {
namespace packets {

GameServerLoginRequest::GameServerLoginRequest(const UnicodeString& name, const UnicodeString& password, uint32_t key) : Packet(0x91, 65),
    name_(name), password_(password), key_(key) {
}

bool GameServerLoginRequest::write(int8_t* buf, unsigned int len, unsigned int& index) const {
    bool ret = true;

    ret = ret && writePacketId(buf, len, index);
    ret = ret && PacketWriter::write(buf, len, index, key_);
    ret = ret && PacketWriter::writeUtf8Fixed(buf, len, index, name_, 30);
    ret = ret && PacketWriter::writeUtf8Fixed(buf, len, index, password_, 30);

    return ret;
}

}
}
}
