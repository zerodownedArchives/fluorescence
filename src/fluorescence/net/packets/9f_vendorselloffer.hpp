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


#ifndef FLUO_NET_PACKETS_VENDORSELLOFFER_HPP
#define FLUO_NET_PACKETS_VENDORSELLOFFER_HPP

#include <net/packet.hpp>

#include <vector>

#include <typedefs.hpp>

namespace fluo {
namespace net {

namespace packets {

struct VendorSellOfferEntry {
    Serial serial_;
    uint16_t amount_;
};

class VendorSellOffer : public Packet {
public:
    VendorSellOffer(Serial vendorSerial, const std::vector<VendorSellOfferEntry>& entries);

    virtual bool write(int8_t* buf, unsigned int len, unsigned int& index) const;

private:
    Serial vendorSerial_;
    std::vector<VendorSellOfferEntry> entries_;
};

}
}
}

#endif
