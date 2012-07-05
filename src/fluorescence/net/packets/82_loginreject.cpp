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



#include "82_loginreject.hpp"

#include <ui/gumpmenus.hpp>
#include <net/manager.hpp>

namespace fluo {
namespace net {
namespace packets {

LoginReject::LoginReject() : Packet(0x82, 2) {
}


bool LoginReject::read(const int8_t* buf, unsigned int len, unsigned int& index) {
    bool ret = true;

    ret &= PacketReader::read(buf, len, index, reason_);

    return ret;
}

void LoginReject::onReceive() {
    UnicodeString reasonStr;
    switch (reason_) {
    case 0x00:
        reasonStr = "Login denied: unknown user";
        break;
    case 0x01:
        reasonStr = "Login denied: account in use";
        break;
    case 0x02:
        reasonStr = "Login denied: account disabled";
        break;
    case 0x03:
        reasonStr = "Login denied: bad password";
        break;
    default:
        reasonStr = "Login denied: communication error";
        break;
    }

    ui::GumpMenus::openMessageBox(reasonStr);

    net::Manager::getSingleton()->disconnect();
}

}
}
}
