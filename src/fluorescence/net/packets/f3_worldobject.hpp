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


#ifndef FLUO_NET_PACKETS_WORLDOBJECT_HPP
#define FLUO_NET_PACKETS_WORLDOBJECT_HPP

#include <net/packet.hpp>

#include <typedefs.hpp>

namespace fluo {
namespace net {

namespace packets {

class WorldObject : public Packet {
public:
    WorldObject();

    virtual bool read(const int8_t* buf, unsigned int len, unsigned int& index);

    virtual void onReceive();

private:
	uint8_t datatype_; // 0x00 = use TileData, 0x01 = use BodyData, 0x02 = use MultiData
    Serial serial_;
	uint16_t objectid_;
	uint8_t direction_;
	uint16_t amount1_;
	uint16_t amount2_;
	uint16_t locX_;
    uint16_t locY_;
	int8_t locZ_;
	uint8_t quality_; // Light Level (TileData.Quality or 0 for Mobiles)
	uint16_t hue_;
	uint8_t flag_; // 0x20 = Show Properties; 0x80 = Hidden
	uint16_t access_; // for items only, 0x01 = Player Item, 0x00 = World Item
};

}
}
}

#endif
