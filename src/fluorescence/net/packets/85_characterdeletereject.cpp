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



#include "85_characterdeletereject.hpp"

#include <ui/gumpmenus.hpp>
#include <net/manager.hpp>

namespace fluo {
namespace net {
namespace packets {

CharacterDeleteReject::CharacterDeleteReject() : Packet(0x85, 2) {
}


bool CharacterDeleteReject::read(const int8_t* buf, unsigned int len, unsigned int& index) {
    bool ret = true;

    ret &= PacketReader::read(buf, len, index, reason_);

    return ret;
}

void CharacterDeleteReject::onReceive() {
    UnicodeString reasonStr;
    switch (reason_) {
    case 0x00:
        reasonStr = "Character delete denied: Invalid password";
        break;
    case 0x01:
        reasonStr = "Character delete denied: Character does not exist";
        break;
    case 0x02:
        reasonStr = "Character delete denied: Character is in game right now";
        break;
    case 0x03:
        reasonStr = "Character delete denied: Character is not old enough to delete";
        break;
    case 0x04:
        reasonStr = "Character delete denied: Character is currently queued for backup";
        break;
    default:
        reasonStr = "Character delete denied: Server could not carry out your request";
        break;
    }

    ui::GumpMenus::openMessageBox(reasonStr);
}

}
}
}
