
#include "singleclick.hpp"

namespace uome {
namespace net {
namespace packets {

SingleClick::SingleClick(Serial serial) : Packet(0x09, 5), serial_(serial) {
}

bool SingleClick::write(int8_t* buf, unsigned int len, unsigned int& index) const {
    bool ret = true;

    ret &= writePacketId(buf, len, index);
    ret &= PacketWriter::write(buf, len, index, serial_);

    return ret;
}

}
}
}
