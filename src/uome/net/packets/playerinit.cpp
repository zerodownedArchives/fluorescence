
#include "playerinit.hpp"

#include <world/manager.hpp>

namespace uome {
namespace net {
namespace packets {

PlayerInit::PlayerInit() : Packet(0x1b, 37) {
}

bool PlayerInit::read(const int8_t* buf, unsigned int len, unsigned int& index) {
    bool ret = true;

    ret &= PacketReader::read(buf, len, index, serial_);
    index += 4;
    ret &= PacketReader::read(buf, len, index, bodyId_);
    ret &= PacketReader::read(buf, len, index, locX_);
    ret &= PacketReader::read(buf, len, index, locY_);
    index += 1;
    ret &= PacketReader::read(buf, len, index, locZ_);
    ret &= PacketReader::read(buf, len, index, direction_);
    index += 19;

    return ret;
}

void PlayerInit::onReceive() {
    world::Manager::getSingleton()->initPlayer(this);
}

}
}
}
