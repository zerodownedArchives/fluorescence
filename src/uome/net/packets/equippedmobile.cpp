
#include "equippedmobile.hpp"

#include <world/manager.hpp>
#include <world/mobile.hpp>
#include <world/dynamicitem.hpp>

namespace uome {
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

    mob->setLocation(locX_, locY_, locZ_);
    mob->setBodyId(bodyId_);
    mob->setDirection(direction_);
    mob->setHue(hue_);

    std::list<EquipInfo>::const_iterator iter = equipment_.begin();
    std::list<EquipInfo>::const_iterator end = equipment_.end();

    for (; iter != end; ++iter) {
        boost::shared_ptr<world::DynamicItem> itm = world::Manager::getSingleton()->getDynamicItem(iter->serial_);
        itm->setArtId(iter->artId_);
        itm->setHue(iter->hue_);
        itm->setLayer(iter->layer_);

        mob->equip(itm);

        itm->shared_from_this();
    }
}

}
}
}
