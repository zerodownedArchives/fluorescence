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



#include "f3_worldobject.hpp"

#include <net/manager.hpp>
#include <misc/log.hpp>
#include <ui/manager.hpp>
#include <world/manager.hpp>
#include <world/dynamicitem.hpp>

namespace fluo {
namespace net {
namespace packets {

WorldObject::WorldObject() : Packet(0xF3, net::Manager::getSingleton()->getProtocolVersion() >= ProtocolVersion::HS ? 26 : 24) {
}

bool WorldObject::read(const int8_t* buf, unsigned int len, unsigned int& index) {
    bool ret = true;

	uint16_t header;

	ret = ret && PacketReader::read(buf, len, index, header);
	if (header != 0x0001) {
        LOG_ERROR << "Unknown header != 1 in WorldObject packet" << std::endl;
		return false;
    }
	ret = ret && PacketReader::read(buf, len, index, datatype_);
	ret = ret && PacketReader::read(buf, len, index, serial_);
	ret = ret && PacketReader::read(buf, len, index, objectid_);
	ret = ret && PacketReader::read(buf, len, index, direction_);
	ret = ret && PacketReader::read(buf, len, index, amount1_);
	ret = ret && PacketReader::read(buf, len, index, amount2_);
	ret = ret && PacketReader::read(buf, len, index, locX_);
	ret = ret && PacketReader::read(buf, len, index, locY_);
	ret = ret && PacketReader::read(buf, len, index, locZ_);
	ret = ret && PacketReader::read(buf, len, index, quality_);
	ret = ret && PacketReader::read(buf, len, index, hue_);
	ret = ret && PacketReader::read(buf, len, index, flag_);

	if (net::Manager::getSingleton()->getProtocolVersion() >= ProtocolVersion::HS) {
		ret = ret && PacketReader::read(buf, len, index, access_);
	} else {
		access_ = 0;
	}

    return ret;
}

void WorldObject::onReceive() {
	if (datatype_ == 0x00) {
		// dynamic item
		boost::shared_ptr<world::DynamicItem> itm = world::Manager::getSingleton()->getDynamicItem(serial_);

		itm->setLocation(locX_, locY_, locZ_);
		itm->setDirection(direction_);
		itm->setAmount(amount1_);
		itm->setStackIdOffset(0);
		itm->setArtId(objectid_);
		itm->setHue(hue_);

		// TODO: handle status
	} else {
		LOG_ERROR << "Unable to handle WorldObject packets with datatype != 0" << std::endl;
	}
}

}
}
}
