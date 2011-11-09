
#include "movementrequest.hpp"

namespace fluo {
namespace net {
namespace packets {

MovementRequest::MovementRequest(uint8_t direction, uint8_t sequence, uint32_t fastWalkKey) : Packet(0x02, 7),
        direction_(direction), sequence_(sequence), fastWalkKey_(fastWalkKey) {
}

bool MovementRequest::write(int8_t* buf, unsigned int len, unsigned int& index) const {
    bool ret = true;

    ret &= writePacketId(buf, len, index);

    ret &= PacketWriter::write(buf, len, index, direction_);
    ret &= PacketWriter::write(buf, len, index, sequence_);
    ret &= PacketWriter::write(buf, len, index, fastWalkKey_);

    return ret;
}

}
}
}
