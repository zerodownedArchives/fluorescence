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



#include "1a_worlditem.hpp"

#include <world/manager.hpp>
#include <world/dynamicitem.hpp>

#include <ui/manager.hpp>

namespace fluo {
namespace net {
namespace packets {

WorldItem::WorldItem() : Packet(0x1A) {
}

bool WorldItem::read(const int8_t* buf, unsigned int len, unsigned int& index) {
    bool ret = true;

    bool readAmount = false;
    bool readStackIdOffset = false;
    bool readDirection = false;
    bool readHue = false;
    bool readStatus = false;

    ret = ret && PacketReader::read(buf, len, index, serial_);
    if (serial_ & 0x80000000) {
        readAmount = true;
        serial_ &= ~0x80000000;
    }

    ret = ret && PacketReader::read(buf, len, index, artId_);
    if (artId_ & 0x8000) {
        readStackIdOffset = true;
        artId_ &= ~0x8000;
    }

    if (readAmount) {
        ret = ret && PacketReader::read(buf, len, index, amount_);
    } else {
        amount_ = 1;
    }

    if (readStackIdOffset) {
        ret = ret && PacketReader::read(buf, len, index, stackIdOffset_);
    } else {
        stackIdOffset_ = 0;
    }

    ret = ret && PacketReader::read(buf, len, index, locX_);
    if (locX_ & 0x8000) {
        readDirection = true;
        locX_ &= ~0x8000;
    }

    ret = ret && PacketReader::read(buf, len, index, locY_);
    if (locY_ & 0x8000) {
        readHue = true;
        locY_ &= ~0x8000;
    }
    if (locY_ & 0x4000) {
        readStatus = true;
        locY_ &= ~0x4000;
    }

    if (readDirection) {
        ret = ret && PacketReader::read(buf, len, index, direction_);
    } else {
        direction_ = 0;
    }

    ret = ret && PacketReader::read(buf, len, index, locZ_);

    if (readHue) {
        ret = ret && PacketReader::read(buf, len, index, hue_);
    } else {
        hue_ = 0;
    }

    if (readStatus) {
        ret = ret && PacketReader::read(buf, len, index, status_);
    } else {
        status_ = 0;
    }

    return ret;
}

void WorldItem::onReceive() {
    boost::shared_ptr<world::DynamicItem> itm = world::Manager::getSingleton()->getDynamicItem(serial_);

    itm->setLocation(locX_, locY_, locZ_);
    itm->setDirection(direction_);
    itm->setAmount(amount_);
    itm->setStackIdOffset(stackIdOffset_);
    itm->setArtId(artId_);
    itm->setHue(hue_);

    // TODO: handle status
}

}
}
}
