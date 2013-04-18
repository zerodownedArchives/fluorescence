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



#include "d6_objectproperties.hpp"

#include <data/manager.hpp>
#include <data/clilocloader.hpp>
#include <misc/log.hpp>
#include <world/manager.hpp>
#include <world/dynamicitem.hpp>
#include <world/mobile.hpp>

namespace fluo {
namespace net {
namespace packets {

ObjectProperties::ObjectProperties() : Packet(0xD6) {
}

bool ObjectProperties::read(const int8_t* buf, unsigned int len, unsigned int& index) {
    bool ret = true;

    // skip 0x0001
    index += 2;

    ret &= PacketReader::read(buf, len, index, serial_);

    // skip 0x0000
    index += 2;

    ret &= PacketReader::read(buf, len, index, hash_);

    while (true) {
        uint32_t cliloc;
        ret &= PacketReader::read(buf, len, index, cliloc);
        if (cliloc == 0x00000000) {
            break;
        }

        //LOG_DEBUG << "cliloc: " << cliloc << std::endl;

        Property newProp;
        newProp.cliloc_ = cliloc;

        uint16_t argsLen;
        ret &= PacketReader::read(buf, len, index, argsLen);
        if (argsLen > 0) {
            ret &= PacketReader::readUnicodeFixedLE(buf, len, index, newProp.args_, argsLen / 2);
        }

        //LOG_DEBUG << "argsLen=" << argsLen << " args=" << newProp.args_ << std::endl;

        props_.push_back(newProp);
    }

    return ret;
}

void ObjectProperties::onReceive() {
    //LOG_DEBUG << "Object properties for serial " << serial_ << std::endl;

    std::list<Property>::iterator iter = props_.begin();
    std::list<Property>::iterator end = props_.end();
    //for (; iter != end; ++iter) {
        //LOG_DEBUG << "  " << data::Manager::getClilocLoader()->get(iter->cliloc_, iter->args_) << std::endl;
    //}

    //iter = props_.begin();

    boost::shared_ptr<world::DynamicItem> itm = world::Manager::getSingleton()->getDynamicItem(serial_, false);
    if (itm) {
        itm->clearClilocProperties();
        for (; iter != end; ++iter) {
            itm->addClilocProperty(iter->cliloc_, iter->args_);
        }
        itm->setClilocPropertiesHash(hash_);
    } else {
        boost::shared_ptr<world::Mobile> mob = world::Manager::getSingleton()->getMobile(serial_, false);
        if (mob) {
            mob->clearClilocProperties();
            for (; iter != end; ++iter) {
                mob->addClilocProperty(iter->cliloc_, iter->args_);
            }
            mob->setClilocPropertiesHash(hash_);
        } else {
            LOG_WARN << "Cliloc properties for unknown object " << serial_ << std::endl;
        }
    }
}

}
}
}
