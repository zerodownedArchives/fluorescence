
#include "statupdatehitpoints.hpp"

#include <world/manager.hpp>
#include <world/mobile.hpp>

#include <misc/log.hpp>

namespace fluo {
namespace net {
namespace packets {

StatUpdateHitpoints::StatUpdateHitpoints() : Packet(0xA1, 9) {
}

bool StatUpdateHitpoints::read(const int8_t* buf, unsigned int len, unsigned int& index) {
    bool ret = true;

    ret &= PacketReader::read(buf, len, index, serial_);
    ret &= PacketReader::read(buf, len, index, maximum_);
    ret &= PacketReader::read(buf, len, index, current_);

    return ret;
}

void StatUpdateHitpoints::onReceive() {
    boost::shared_ptr<world::Mobile> mob = world::Manager::getSingleton()->getMobile(serial_, false);
    if (mob) {
        mob->getProperty("hitpoints").setInt(current_);
        mob->getProperty("hitpoints-max").setInt(maximum_);

        mob->onPropertyUpdate();
    } else {
        LOG_WARN << "Received full stat update for unknown mobile serial=" << serial_ << std::endl;
    }
}

}
}
}
