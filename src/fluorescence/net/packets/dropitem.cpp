
#include "dropitem.hpp"

#include <world/dynamicitem.hpp>

namespace fluo {
namespace net {
namespace packets {

DropItem::DropItem(const world::DynamicItem* itm, boost::shared_ptr<world::DynamicItem> target) : Packet(0x08, 14),
    serial_(itm->getSerial()), locX_(target->getLocX()), locY_(target->getLocY()), locZ_(target->getLocZ()), containerSerial_(target->getSerial()) {
}

DropItem::DropItem(const world::DynamicItem* itm, unsigned int x, unsigned int y, int z) : Packet(0x08, 14),
    serial_(itm->getSerial()), locX_(x), locY_(y), locZ_(z), containerSerial_(0xFFFFFFFF) {
}

bool DropItem::write(int8_t* buf, unsigned int len, unsigned int& index) const {
    bool ret = true;

    ret &= writePacketId(buf, len, index);
    ret &= PacketWriter::write(buf, len, index, serial_);
    ret &= PacketWriter::write(buf, len, index, locX_);
    ret &= PacketWriter::write(buf, len, index, locY_);
    ret &= PacketWriter::write(buf, len, index, locZ_);
    ret &= PacketWriter::write(buf, len, index, containerSerial_);

    return ret;
}

}
}
}
