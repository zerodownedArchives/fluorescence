
#include "contextmenureply.hpp"

namespace fluo {
namespace net {
namespace packets {
namespace bf {

ContextMenuReply::ContextMenuReply(Serial serial, uint16_t choice) : Packet(0x15), serial_(serial), choice_(choice) {
}

bool ContextMenuReply::write(int8_t* buf, unsigned int len, unsigned int& index) const {
    bool ret = true;

    ret &= writeSubPacketId(buf, len, index);

    ret &= PacketWriter::write(buf, len, index, serial_);
    ret &= PacketWriter::write(buf, len, index, choice_);

    return ret;
}

}
}
}
}
