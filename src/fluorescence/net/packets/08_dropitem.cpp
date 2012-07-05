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



#include "08_dropitem.hpp"

#include <world/dynamicitem.hpp>
#include <world/serverobject.hpp>

namespace fluo {
namespace net {
namespace packets {

DropItem::DropItem(const world::DynamicItem* itm, boost::shared_ptr<world::ServerObject> target) : Packet(0x08, 14),
    serial_(itm->getSerial()), locX_(0xFFFF), locY_(0xFFFF), locZ_(0), containerSerial_(target->getSerial()) {
}

DropItem::DropItem(const world::DynamicItem* itm, unsigned int x, unsigned int y, int z) : Packet(0x08, 14),
    serial_(itm->getSerial()), locX_(x), locY_(y), locZ_(z), containerSerial_(0xFFFFFFFF) {
}

DropItem::DropItem(const world::DynamicItem* itm, unsigned int x, unsigned int y, int z, boost::shared_ptr<world::ServerObject> target) : Packet(0x08, 14),
    serial_(itm->getSerial()), locX_(x), locY_(y), locZ_(z), containerSerial_(target->getSerial()) {
}

bool DropItem::write(int8_t* buf, unsigned int len, unsigned int& index) const {
    bool ret = true;

    ret &= writePacketId(buf, len, index);
    ret &= PacketWriter::write(buf, len, index, serial_);
    ret &= PacketWriter::write(buf, len, index, locX_);
    ret &= PacketWriter::write(buf, len, index, locY_);
    ret &= PacketWriter::write(buf, len, index, locZ_);
    ret &= PacketWriter::write(buf, len, index, containerSerial_);

    return ret;
}

}
}
}
