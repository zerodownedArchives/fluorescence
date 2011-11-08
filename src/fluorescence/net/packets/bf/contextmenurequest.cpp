
#include "contextmenurequest.hpp"

namespace fluo {
namespace net {
namespace packets {
namespace bf {

ContextMenuRequest::ContextMenuRequest(Serial serial) : Packet(0x13), serial_(serial) {
}

bool ContextMenuRequest::write(int8_t* buf, unsigned int len, unsigned int& index) const {
    bool ret = true;

    ret &= writeSubPacketId(buf, len, index);

    ret &= PacketWriter::write(buf, len, index, serial_);

    return ret;
}

}
}
}
}
