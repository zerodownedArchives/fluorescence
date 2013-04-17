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



#include "dc_objectpropertieshash.hpp"

#include "d6_objectpropertyrequest.hpp"

#include <misc/log.hpp>
#include <net/manager.hpp>
#include <world/manager.hpp>
#include <world/mobile.hpp>
#include <world/dynamicitem.hpp>

namespace fluo {
namespace net {
namespace packets {

ObjectPropertiesHash::ObjectPropertiesHash() : Packet(0xDC, 9) {
}

bool ObjectPropertiesHash::read(const int8_t* buf, unsigned int len, unsigned int& index) {
    bool ret = true;

    ret &= PacketReader::read(buf, len, index, serial_);
    ret &= PacketReader::read(buf, len, index, hash_);

    return ret;
}

void ObjectPropertiesHash::onReceive() {
    boost::shared_ptr<world::DynamicItem> itm = world::Manager::getSingleton()->getDynamicItem(serial_, false);
    if (itm) {
        itm->requestClilocProperties();
    } else {
        boost::shared_ptr<world::Mobile> mob = world::Manager::getSingleton()->getMobile(serial_, false);
        if (mob) {
            if (!mob->checkClilocPropertiesHash(hash_)) {
                mob->requestClilocProperties();
            }
        } else {
            LOG_WARN << "Cliloc properties hash update for unknown object " << serial_ << std::endl;
        }
    }
}

}
}
}
