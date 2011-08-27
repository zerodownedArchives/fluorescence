
#include "statupdatefatigue.hpp"

#include <world/manager.hpp>
#include <world/mobile.hpp>

namespace uome {
namespace net {
namespace packets {

StatUpdateFatigue::StatUpdateFatigue() : Packet(0xA1, 9) {
}

bool StatUpdateFatigue::read(const int8_t* buf, unsigned int len, unsigned int& index) {
    bool ret = true;

    ret &= PacketReader::read(buf, len, index, serial_);
    ret &= PacketReader::read(buf, len, index, maximum_);
    ret &= PacketReader::read(buf, len, index, current_);

    return ret;
}

void StatUpdateFatigue::onReceive() {
    boost::shared_ptr<world::Mobile> mob = world::Manager::getSingleton()->getMobile(serial_, false);
    if (mob) {
        mob->getProperty("fatigue-current").setInt(current_);
        mob->getProperty("fatigue-max").setInt(maximum_);

        mob->onPropertyUpdate();
    }
}

}
}
}
