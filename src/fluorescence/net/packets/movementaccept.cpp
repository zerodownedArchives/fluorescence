
#include "movementaccept.hpp"

#include <net/manager.hpp>
#include <net/walkmanager.hpp>

namespace fluo {
namespace net {
namespace packets {

MovementAccept::MovementAccept() : Packet(0x22, 3), sequence_(0), status_(0) {
}

bool MovementAccept::write(int8_t* buf, unsigned int len, unsigned int& index) const {
    bool ret = true;

    ret &= writePacketId(buf, len, index);
    ret &= PacketWriter::write(buf, len, index, sequence_);
    ret &= PacketWriter::write(buf, len, index, status_);

    return ret;
}

bool MovementAccept::read(const int8_t* buf, unsigned int len, unsigned int& index) {
    bool ret = true;

    ret &= PacketReader::read(buf, len, index, sequence_);
    ret &= PacketReader::read(buf, len, index, status_);

    return ret;
}

void MovementAccept::onReceive() {
    net::Manager::getWalkManager()->onMovementAccept(sequence_);

    // TODO: handle status
}

}
}
}
