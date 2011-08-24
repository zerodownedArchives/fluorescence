
#include "pickupitem.hpp"

#include <world/dynamicitem.hpp>

namespace uome {
namespace net {
namespace packets {

PickUpItem::PickUpItem(const world::DynamicItem* itm, unsigned int amount) : Packet(0x07, 7),
    serial_(itm->getSerial()), amount_(amount) {
}

bool PickUpItem::write(int8_t* buf, unsigned int len, unsigned int& index) const {
    bool ret = true;

    ret &= writePacketId(buf, len, index);
    ret &= PacketWriter::write(buf, len, index, serial_);
    ret &= PacketWriter::write(buf, len, index, amount_);

    return ret;
}

}
}
}
