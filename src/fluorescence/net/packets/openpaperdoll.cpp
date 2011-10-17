
#include "openpaperdoll.hpp"

#include <boost/shared_ptr.hpp>

#include <world/manager.hpp>
#include <world/mobile.hpp>
#include <misc/log.hpp>

namespace fluo {
namespace net {
namespace packets {

OpenPaperdoll::OpenPaperdoll() : Packet(0x88, 66) {
}

bool OpenPaperdoll::read(const int8_t* buf, unsigned int len, unsigned int& index) {
    bool ret = true;

    ret &= PacketReader::read(buf, len, index, serial_);
    ret &= PacketReader::readUtf8Fixed(buf, len, index, name_, 60);
    ret &= PacketReader::read(buf, len, index, status_);

    return ret;
}

void OpenPaperdoll::onReceive() {
    boost::shared_ptr<world::Mobile> mob = world::Manager::getSingleton()->getMobile(serial_, false);
    if (mob) {
        mob->getProperty("paperdoll-name").setString(name_);
        mob->onPropertyUpdate();

        mob->openPaperdoll();

        // TODO: handle status
    } else {
        LOG_WARN << "Received open paperdoll packet for unknown mobile serial=" << serial_ << std::endl;
    }
}

}
}
}
