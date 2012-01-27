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



#include "equippeditem.hpp"

#include <world/manager.hpp>
#include <world/mobile.hpp>
#include <world/dynamicitem.hpp>

#include <misc/log.hpp>

namespace fluo {
namespace net {
namespace packets {

EquippedItem::EquippedItem() : Packet(0x2E, 15) {
}

bool EquippedItem::read(const int8_t* buf, unsigned int len, unsigned int& index) {
    bool ret = true;

    ret = ret && PacketReader::read(buf, len, index, serial_);
    ret = ret && PacketReader::read(buf, len, index, artId_);
    ++index; // jump unknown byte
    ret = ret && PacketReader::read(buf, len, index, layer_);
    ret = ret && PacketReader::read(buf, len, index, mobile_);
    ret = ret && PacketReader::read(buf, len, index, hue_);

    return ret;
}

void EquippedItem::onReceive() {
    boost::shared_ptr<world::Mobile> mob = world::Manager::getSingleton()->getMobile(mobile_, false);

    if (!mob) {
        LOG_WARN << "Equipment update for not existing mobile serial=" << mobile_ << std::endl;
        return;
    }

    boost::shared_ptr<world::DynamicItem> itm = world::Manager::getSingleton()->getDynamicItem(serial_);
    itm->setArtId(artId_);
    itm->setLayer(layer_);
    itm->setHue(hue_);

    mob->addChildObject(itm);
}

}
}
}
