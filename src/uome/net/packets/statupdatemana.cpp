
#include "statupdatemana.hpp"

#include <world/manager.hpp>
#include <world/mobile.hpp>

namespace uome {
namespace net {
namespace packets {

StatUpdateMana::StatUpdateMana() : Packet(0xA2, 9) {
}

bool StatUpdateMana::read(const int8_t* buf, unsigned int len, unsigned int& index) {
    bool ret = true;

    ret &= PacketReader::read(buf, len, index, serial_);
    ret &= PacketReader::read(buf, len, index, maximum_);
    ret &= PacketReader::read(buf, len, index, current_);

    return ret;
}

void StatUpdateMana::onReceive() {
    boost::shared_ptr<world::Mobile> mob = world::Manager::getSingleton()->getMobile(serial_, false);
    if (mob) {
        mob->getProperty("mana-current").setInt(current_);
        mob->getProperty("mana-max").setInt(maximum_);

        mob->onPropertyUpdate();
    }
}

}
}
}
