
#include "fastwalkupdate.hpp"

#include <net/manager.hpp>
#include <net/walkmanager.hpp>

namespace fluo {
namespace net {
namespace packets {
namespace bf {

FastWalkUpdate::FastWalkUpdate() : Packet(0x02) {
}

bool FastWalkUpdate::read(const int8_t* buf, unsigned int len, unsigned int& index) {
    bool ret = true;

    ret &= PacketReader::read(buf, len, index, key_);

    return ret;
}

void FastWalkUpdate::onReceive() {
    net::Manager::getWalkManager()->updateFastWalkStack(key_);
}

}
}
}
}
