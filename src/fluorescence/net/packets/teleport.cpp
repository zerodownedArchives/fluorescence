
#include "teleport.hpp"

#include <world/manager.hpp>
#include <world/mobile.hpp>

namespace fluo {
namespace net {
namespace packets {

Teleport::Teleport() : Packet(0x20, 19) {
}

bool Teleport::read(const int8_t* buf, unsigned int len, unsigned int& index) {
    bool ret = true;

    ret &= PacketReader::read(buf, len, index, serial_);
    ret &= PacketReader::read(buf, len, index, bodyId_);
    ++index;
    ret &= PacketReader::read(buf, len, index, hue_);
    ret &= PacketReader::read(buf, len, index, status_);
    ret &= PacketReader::read(buf, len, index, locX_);
    ret &= PacketReader::read(buf, len, index, locY_);
    index += 2;
    ret &= PacketReader::read(buf, len, index, direction_);
    ret &= PacketReader::read(buf, len, index, locZ_);

    return ret;
}

void Teleport::onReceive() {
    boost::shared_ptr<world::Mobile> player = world::Manager::getSingleton()->getPlayer();

    if (player && player->getSerial() == serial_) {
        player->setLocation(locX_, locY_, locZ_);
        player->setBodyId(bodyId_);
        player->setDirection(direction_);
        player->setHue(hue_);

        // TODO: handle status
    }
}

}
}
}
