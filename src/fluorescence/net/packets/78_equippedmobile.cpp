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



#include "78_equippedmobile.hpp"

#include <world/manager.hpp>
#include <world/mobile.hpp>
#include <world/dynamicitem.hpp>
#include <world/smoothmovement.hpp>
#include <world/smoothmovementmanager.hpp>

#include <ui/manager.hpp>

namespace fluo {
namespace net {
namespace packets {

EquippedMobile::EquippedMobile() : Packet(0x78) {
}

bool EquippedMobile::read(const int8_t* buf, unsigned int len, unsigned int& index) {
    bool ret = true;

    ret = ret && PacketReader::read(buf, len, index, serial_);
    ret = ret && PacketReader::read(buf, len, index, bodyId_);
    ret = ret && PacketReader::read(buf, len, index, locX_);
    ret = ret && PacketReader::read(buf, len, index, locY_);
    ret = ret && PacketReader::read(buf, len, index, locZ_);
    ret = ret && PacketReader::read(buf, len, index, direction_);
    ret = ret && PacketReader::read(buf, len, index, hue_);
    ret = ret && PacketReader::read(buf, len, index, status_);
    ret = ret && PacketReader::read(buf, len, index, notoriety_);

    EquipInfo curEntry;
    ret = ret && PacketReader::read(buf, len, index, curEntry.serial_);

    while (ret && curEntry.serial_ != 0x0) {
        ret = ret && PacketReader::read(buf, len, index, curEntry.artId_);
        ret = ret && PacketReader::read(buf, len, index, curEntry.layer_);

        if (curEntry.artId_ & 0x8000) {
            ret = ret && PacketReader::read(buf, len, index, curEntry.hue_);
            curEntry.artId_ &= ~0x8000;
        } else {
            curEntry.hue_ = 0;
        }

        equipment_.push_back(curEntry);

        ret = ret && PacketReader::read(buf, len, index, curEntry.serial_);
    }

    return ret;
}

void EquippedMobile::onReceive() {
    boost::shared_ptr<world::Mobile> mob = world::Manager::getSingleton()->getMobile(serial_);

    mob->setBodyId(bodyId_);
    mob->setDirection(direction_);

    int diffx = mob->getLocXGame();
    diffx -= locX_;
    diffx = abs(diffx);

    int diffy = mob->getLocYGame();
    diffy -= locY_;
    diffy = abs(diffy);

    if ((diffx == 0 && diffy == 0) || diffx > 1 || diffy > 1) {
        // far teleport, or no teleport
        mob->setLocation(locX_, locY_, locZ_);
    } else {
        world::SmoothMovement mov(mob, CL_Vec3f(locX_, locY_, locZ_), mob->getMovementDuration());
        world::Manager::getSmoothMovementManager()->add(mob->getSerial(), mov);
    }

    mob->setHue(hue_);

    std::list<EquipInfo>::const_iterator iter = equipment_.begin();
    std::list<EquipInfo>::const_iterator end = equipment_.end();

    for (; iter != end; ++iter) {
        boost::shared_ptr<world::DynamicItem> itm = world::Manager::getSingleton()->getDynamicItem(iter->serial_);
        itm->setArtId(iter->artId_);
        itm->setHue(iter->hue_);
        itm->setLayer(iter->layer_);

        mob->addChildObject(itm);
    }
}

}
}
}
