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



#include "clientversion.hpp"

#include <net/manager.hpp>

#include <client.hpp>
#include <misc/config.hpp>
#include <misc/log.hpp>

namespace fluo {
namespace net {
namespace packets {

ClientVersion::ClientVersion() : Packet(0xBD) {
}

ClientVersion::ClientVersion(const UnicodeString& version) : Packet(0xBD), version_(version) {
}

bool ClientVersion::write(int8_t* buf, unsigned int len, unsigned int& index) const {
    bool ret = true;

    ret &= writePacketId(buf, len, index);
    unsigned int sizeIndex = preparePacketSize(index);

    ret &= PacketWriter::writeUtf8Null(buf, len, index, version_);

    writePacketSize(buf, len, index, sizeIndex);

    return ret;
}

bool ClientVersion::read(const int8_t* buf, unsigned int len, unsigned int& index) {
    // nothing to read here
    return true;
}

void ClientVersion::onReceive() {
    UnicodeString version = Client::getSingleton()->getConfig()["/fluo/shard/client@version-id"].asString();
    LOG_INFO << "Identifying myself as " << version << std::endl;
    ClientVersion reply(version);
    net::Manager::getSingleton()->send(reply);
}

}
}
}
