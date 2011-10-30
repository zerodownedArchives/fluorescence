
#include "displaycontainer.hpp"

#include <boost/shared_ptr.hpp>

#include <world/manager.hpp>
#include <world/dynamicitem.hpp>

#include <misc/log.hpp>

namespace fluo {
namespace net {
namespace packets {

DisplayContainer::DisplayContainer() : Packet(0x24, 7) {
}

bool DisplayContainer::read(const int8_t* buf, unsigned int len, unsigned int& index) {
    bool ret = true;

    ret &= PacketReader::read(buf, len, index, serial_);
    ret &= PacketReader::read(buf, len, index, gumpId_);

    return ret;
}

void DisplayContainer::onReceive() {
    boost::shared_ptr<world::DynamicItem> itm = world::Manager::getSingleton()->getDynamicItem(serial_, false);
    if (itm) {
        itm->openContainerGump(gumpId_);
    } else {
        LOG_WARN << "Trying to open container with unknown serial " << serial_;
    }
}

}
}
}
