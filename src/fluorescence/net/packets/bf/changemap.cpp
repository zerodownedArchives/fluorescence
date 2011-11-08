
#include "changemap.hpp"

#include <world/manager.hpp>

namespace fluo {
namespace net {
namespace packets {
namespace bf {

ChangeMap::ChangeMap() : Packet(0x08) {
}

bool ChangeMap::read(const int8_t* buf, unsigned int len, unsigned int& index) {
    bool ret = true;

    ret &= PacketReader::read(buf, len, index, mapId_);

    return ret;
}

void ChangeMap::onReceive() {
    world::Manager::getSingleton()->setCurrentMapId(mapId_);
}

}
}
}
}
