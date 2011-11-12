
#include "movementdeny.hpp"

#include <net/manager.hpp>
#include <net/walkmanager.hpp>

namespace fluo {
namespace net {
namespace packets {

MovementDeny::MovementDeny() : Packet(0x21, 8) {
}

bool MovementDeny::read(const int8_t* buf, unsigned int len, unsigned int& index) {
    bool ret = true;

    ret &= PacketReader::read(buf, len, index, sequence_);
    ret &= PacketReader::read(buf, len, index, locX_);
    ret &= PacketReader::read(buf, len, index, locY_);
    ret &= PacketReader::read(buf, len, index, direction_);
    ret &= PacketReader::read(buf, len, index, locZ_);

    return ret;
}

void MovementDeny::onReceive() {
    net::Manager::getWalkManager()->onMovementDeny(this);
}

}
}
}
