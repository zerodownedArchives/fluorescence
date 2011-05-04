
#include "packet.hpp"

#include <misc/logger.hpp>

namespace uome {
namespace net {

Packet::Packet(uint8_t id, uint16_t size) : id_(id), size_(size) {
}

bool Packet::writePacketInfo(int8_t* buf, unsigned int len, unsigned int& index) const {
    bool ret = true;

    ret &= PacketWriter::write(buf, len, index, id_);

    return ret;
}

bool Packet::write(int8_t* buf, unsigned int len, unsigned int& index) const {
    return writePacketInfo(buf, len, index);
}

bool Packet::read(const int8_t* buf, unsigned int len, unsigned int& index) {
    return true;
}

bool Packet::hasVariableSize() {
    return size_ == 0;
}

uint16_t Packet::getSize() {
    return size_;
}

void Packet::onReceive() {
    LOGARG_ERROR(LOGTYPE_NETWORK, "Received unhandled packet %x", id_);
}

}
}
