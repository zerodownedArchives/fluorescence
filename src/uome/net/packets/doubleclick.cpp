
#include "doubleclick.hpp"

namespace uome {
namespace net {
namespace packets {

DoubleClick::DoubleClick(Serial serial) : Packet(0x06, 5), serial_(serial) {
}

bool DoubleClick::write(int8_t* buf, unsigned int len, unsigned int& index) const {
    bool ret = true;

    ret &= writePacketId(buf, len, index);
    ret &= PacketWriter::write(buf, len, index, serial_);

    return ret;
}

}
}
}
