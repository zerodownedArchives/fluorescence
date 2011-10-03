
#include "deleteobject.hpp"

#include <world/manager.hpp>

namespace fluo {
namespace net {
namespace packets {

DeleteObject::DeleteObject() : Packet(0x1D, 5) {
}

bool DeleteObject::read(const int8_t* buf, unsigned int len, unsigned int& index) {
    bool ret = true;

    ret &= PacketReader::read(buf, len, index, serial_);

    return ret;
}

void DeleteObject::onReceive() {
    world::Manager::getSingleton()->deleteObject(serial_);
}

}
}
}
