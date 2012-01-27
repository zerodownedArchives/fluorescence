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



#include "containerupdate.hpp"

#include <boost/shared_ptr.hpp>

#include <world/manager.hpp>
#include <world/dynamicitem.hpp>

#include <misc/log.hpp>

namespace fluo {
namespace net {
namespace packets {

ContainerUpdate::ContainerUpdate() : Packet(0x25, 20) {
}

bool ContainerUpdate::read(const int8_t* buf, unsigned int len, unsigned int& index) {
    bool ret = true;

    ret &= PacketReader::read(buf, len, index, serial_);
    ret &= PacketReader::read(buf, len, index, artId_);
    ret &= PacketReader::read(buf, len, index, stackIdOffset_);
    ret &= PacketReader::read(buf, len, index, amount_);
    ret &= PacketReader::read(buf, len, index, pxX_);
    ret &= PacketReader::read(buf, len, index, pxY_);

    //if (useGridByte) {
        //++index;
    //}

    ret &= PacketReader::read(buf, len, index, container_);
    ret &= PacketReader::read(buf, len, index, hue_);

    return ret;
}

void ContainerUpdate::onReceive() {
    boost::shared_ptr<world::DynamicItem> itm = world::Manager::getSingleton()->getDynamicItem(serial_);

    boost::shared_ptr<world::DynamicItem> cont = world::Manager::getSingleton()->getDynamicItem(container_, false);
    if (!cont) {
        LOG_WARN << "Received container update for unknown container " << container_ << std::endl;
        return;
    }

    itm->setArtId(artId_);
    itm->setStackIdOffset(stackIdOffset_);
    itm->setAmount(amount_);
    itm->setLocation(pxX_, pxY_, 0);
    itm->setHue(hue_);

    cont->addChildObject(itm);
}

}
}
}
