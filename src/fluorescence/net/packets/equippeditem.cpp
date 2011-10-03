
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

    mob->equip(itm);
}

}
}
}
