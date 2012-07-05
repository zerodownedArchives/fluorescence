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



#include "3c_containercontent.hpp"

#include <boost/shared_ptr.hpp>

#include <world/manager.hpp>
#include <world/dynamicitem.hpp>

#include <misc/log.hpp>

namespace fluo {
namespace net {
namespace packets {

ContainerContent::ContainerContent() : Packet(0x3C) {
}

bool ContainerContent::read(const int8_t* buf, unsigned int len, unsigned int& index) {
    bool ret = true;

    uint16_t itemCount;
    ret &= PacketReader::read(buf, len, index, itemCount);

    for (unsigned int i = 0; i < itemCount; ++i) {
        ContainerContentEntry curEntry;

        ret &= PacketReader::read(buf, len, index, curEntry.serial_);
        ret &= PacketReader::read(buf, len, index, curEntry.artId_);
        ret &= PacketReader::read(buf, len, index, curEntry.stackIdOffset_);
        ret &= PacketReader::read(buf, len, index, curEntry.amount_);
        ret &= PacketReader::read(buf, len, index, curEntry.pxX_);
        ret &= PacketReader::read(buf, len, index, curEntry.pxY_);

        // jump grid byte
        ++index;

        ret &= PacketReader::read(buf, len, index, curEntry.container_);
        ret &= PacketReader::read(buf, len, index, curEntry.hue_);

        entries_.push_back(curEntry);
    }

    return ret;
}

void ContainerContent::onReceive() {
    std::list<ContainerContentEntry>::const_iterator iter = entries_.begin();
    std::list<ContainerContentEntry>::const_iterator end = entries_.end();

    for (; iter != end; ++iter) {
        boost::shared_ptr<world::DynamicItem> itm = world::Manager::getSingleton()->getDynamicItem(iter->serial_);

        // why is the container serial here for each item? is it possible to add items to several containers with this packet?
        // to be safe, get container for each item entry
        boost::shared_ptr<world::DynamicItem> cont = world::Manager::getSingleton()->getDynamicItem(iter->container_, false);
        if (!cont) {
            LOG_WARN << "Received container contents for unknown container " << iter->container_ << std::endl;
            continue;
        }

        itm->setArtId(iter->artId_);
        itm->setStackIdOffset(iter->stackIdOffset_);
        itm->setAmount(iter->amount_);
        itm->setLocation(iter->pxX_, iter->pxY_, 0);
        itm->setHue(iter->hue_);

        cont->addChildObject(itm);
    }
}

}
}
}
