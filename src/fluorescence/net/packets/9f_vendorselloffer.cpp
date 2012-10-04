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



#include "9f_vendorselloffer.hpp"

namespace fluo {
namespace net {
namespace packets {

VendorSellOffer::VendorSellOffer(Serial vendorSerial, const std::vector<VendorSellOfferEntry>& entries) : Packet(0x9f),
        vendorSerial_(vendorSerial), entries_(entries) {
}

bool VendorSellOffer::write(int8_t* buf, unsigned int len, unsigned int& index) const {
    bool ret = true;

    ret &= writePacketId(buf, len, index);

    unsigned int sizeIndex = preparePacketSize(index);

    ret &= PacketWriter::write(buf, len, index, vendorSerial_);
    uint16_t itemCount = entries_.size();
    ret &= PacketWriter::write(buf, len, index, itemCount);
    for (unsigned int i = 0; i < itemCount; ++i) {
        ret &= PacketWriter::write(buf, len, index, entries_[i].serial_);
        ret &= PacketWriter::write(buf, len, index, entries_[i].amount_);
    }

    writePacketSize(buf, len, index, sizeIndex);

    return ret;
}

}
}
}
