
#include "multipurposepacket.hpp"

#include <misc/log.hpp>

namespace fluo {
namespace net {
namespace packets {

MultiPurposePacket::MultiPurposePacket(uint8_t packetId) : Packet(packetId) {
}

MultiPurposePacket::MultiPurposePacket(uint8_t packetId, boost::shared_ptr<Packet> subpacket) : Packet(packetId), subpacket_(subpacket) {
}

bool MultiPurposePacket::write(int8_t* buf, unsigned int len, unsigned int& index) const {
    bool ret = true;

    ret &= writePacketId(buf, len, index);

    unsigned int sizeHelper = preparePacketSize(index);
    ret &= subpacket_->write(buf, len, index);
    ret &= writePacketSize(buf, len, index, sizeHelper);

    return ret;
}

bool MultiPurposePacket::read(const int8_t* buf, unsigned int len, unsigned int& index) {
    uint16_t subId = 0;
    bool ret = PacketReader::read(buf, len, index, subId);

    if (ret) {
        subpacket_ = getSubpacket(subId);
        if (subpacket_) {
            return subpacket_->read(buf, len, index);
        } else {
            LOG_DEBUG << "Unable to extract subpacket " << std::hex << (unsigned int)subId << " in packet " << (unsigned int)id_ << std::dec << std::endl;
        }
    }

    return true; // there is already an error message if something went wrong
}

void MultiPurposePacket::onReceive() {
    if (subpacket_) {
        subpacket_->onReceive();
    } else {
        LOG_DEBUG << "onReceive called without subpacket in packet " << std::hex << (unsigned int)id_ << std::dec << std::endl;
    }
}

}
}
}
