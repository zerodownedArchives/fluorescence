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



#include "9e_vendorselllist.hpp"

#include <ui/gumpmenus.hpp>

namespace fluo {
namespace net {
namespace packets {

VendorSellList::VendorSellList() : Packet(0x9e) {
}

bool VendorSellList::read(const int8_t* buf, unsigned int len, unsigned int& index) {
    bool ret = true;

    ret &= PacketReader::read(buf, len, index, vendorSerial_);
    uint16_t itemCount;
    ret &= PacketReader::read(buf, len, index, itemCount);

    for (unsigned int i = 0; i < itemCount; ++i) {
        VendorSellListEntry cur;
        ret &= PacketReader::read(buf, len, index, cur.serial_);
        ret &= PacketReader::read(buf, len, index, cur.artId_);
        ret &= PacketReader::read(buf, len, index, cur.hue_);
        ret &= PacketReader::read(buf, len, index, cur.amount_);
        ret &= PacketReader::read(buf, len, index, cur.price_);
        uint16_t nameLen;
        ret &= PacketReader::read(buf, len, index, nameLen);
        ret &= PacketReader::readUtf8Fixed(buf, len, index, cur.name_, nameLen);

        entries_.push_back(cur);
    }

    return ret;
}

void VendorSellList::onReceive() {
}

}
}
}
