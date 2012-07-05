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



#include "14_opencontextmenu.hpp"

#include <data/manager.hpp>
#include <data/clilocloader.hpp>

#include <ui/gumpmenus.hpp>

#include <misc/log.hpp>

namespace fluo {
namespace net {
namespace packets {
namespace bf {

OpenContextMenu::OpenContextMenu() : Packet(0x14) {
}

bool OpenContextMenu::read(const int8_t* buf, unsigned int len, unsigned int& index) {
    bool ret = true;

    uint16_t packetType;
    ret &= PacketReader::read(buf, len, index, packetType);

    ret &= PacketReader::read(buf, len, index, serial_);

    uint8_t entryCount;
    ret &= PacketReader::read(buf, len, index, entryCount);

    //LOG_DEBUG << "serial=" << std::hex << serial_ << std::dec << " count=" << (unsigned int)entryCount << " type=" << (unsigned int)packetType << std::endl;

    for (unsigned int i = 0; i < entryCount; ++i) {
        ContextMenuEntry curEntry;

        if (packetType == 1) {
            ret &= PacketReader::read(buf, len, index, curEntry.entryId_);
            uint16_t clilocNr;
            ret &= PacketReader::read(buf, len, index, clilocNr);
            curEntry.clilocId_ = 3000000 + clilocNr;
        } else {
            uint32_t clilocNr;
            ret &= PacketReader::read(buf, len, index, clilocNr);
            curEntry.clilocId_ = clilocNr;
            ret &= PacketReader::read(buf, len, index, curEntry.entryId_);
        }

        ret &= PacketReader::read(buf, len, index, curEntry.flags_);

        if (curEntry.flags_ & 0x20) {
            ret &= PacketReader::read(buf, len, index, curEntry.hue_);
        }

        entries_.push_back(curEntry);
    }

    return ret;
}

void OpenContextMenu::onReceive() {
    //LOG_DEBUG << "Received menu with " << entries_.size() << " entries" << std::endl;
    //std::list<ContextMenuEntry>::iterator iter = entries_.begin();
    //std::list<ContextMenuEntry>::iterator end = entries_.end();

    //for (; iter != end; ++iter) {
        //LOG_DEBUG << "entry id=" << iter->entryId_ << ": " << data::Manager::getClilocLoader()->get(iter->clilocId_) << std::endl;
    //}

    ui::GumpMenus::openContextMenu(this);
}

}
}
}
}
