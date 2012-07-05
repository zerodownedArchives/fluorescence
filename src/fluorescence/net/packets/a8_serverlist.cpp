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



#include "a8_serverlist.hpp"

#include <misc/log.hpp>

#include <ui/manager.hpp>
#include <ui/gumpmenus.hpp>

namespace fluo {
namespace net {
namespace packets {

ServerList::ServerList() : Packet(0xA8) {
}

bool ServerList::read(const int8_t* buf, unsigned int len, unsigned int& index) {
    bool ret = true;

    ret = ret && PacketReader::read(buf, len, index, flags_);

    uint16_t listSize;
    ret = ret && PacketReader::read(buf, len, index, listSize);

    for (unsigned int i = 0; i < listSize && ret; ++i) {
        ServerListEntry entry;
        ret = ret && PacketReader::read(buf, len, index, entry.index_);
        ret = ret && PacketReader::readUtf8Fixed(buf, len, index, entry.name_, 32);
        ret = ret && PacketReader::read(buf, len, index, entry.fullPercentage_);
        ret = ret && PacketReader::read(buf, len, index, entry.timezoneBias_);
        ret = ret && PacketReader::read(buf, len, index, entry.serverAddress_);

        listEntries_.push_back(entry);
    }

    return ret;
}

void ServerList::onReceive() {
    ui::Manager::getSingleton()->closeGumpMenu("login");
    ui::GumpMenus::openServerListGump(this);
}

}
}
}
