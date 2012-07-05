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



#include "27_pickupreject.hpp"

#include <ui/manager.hpp>
#include <ui/cursormanager.hpp>

#include <world/manager.hpp>

namespace fluo {
namespace net {
namespace packets {

PickUpReject::PickUpReject() : Packet(0x27, 2) {
}

bool PickUpReject::read(const int8_t* buf, unsigned int len, unsigned int& index) {
    bool ret = PacketReader::read(buf, len, index, reason_);

    return ret;
}

void PickUpReject::onReceive() {
    ui::Manager* uiMan = ui::Manager::getSingleton();
    world::Manager* worldMan = world::Manager::getSingleton();

    switch (reason_) {
        case 0x00: worldMan->systemMessage("You cannot pick that up."); break;
        case 0x01: worldMan->systemMessage("That is too far away."); break;
        case 0x02: worldMan->systemMessage("That is out of sight."); break;
        case 0x03: worldMan->systemMessage("That item does not belong to you. You will have to steal it."); break;
        case 0x04: worldMan->systemMessage("You are already holding an item."); break;
    }

    uiMan->getCursorManager()->stopDragging();
}

}
}
}
