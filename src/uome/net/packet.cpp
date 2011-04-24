
#include "packet.hpp"

namespace uome {
namespace net {

Packet::Packet(uint32_t id) : id_(id) {
}

bool Packet::writePacketInfo(int8_t* buf, unsigned int len, unsigned int& index) const {
    bool ret = true;

    ret &= PacketWriter::write(buf, len, index, id_);

    return ret;
}

bool Packet::readPacketInfo(const int8_t* buf, unsigned int len, unsigned int& index) {
    bool ret = true;

    ret &= PacketReader::read(buf, len, index, id_);

    return ret;
}

bool Packet::write(int8_t* buf, unsigned int len, unsigned int& index) const {
    return writePacketInfo(buf, len, index);
}

bool Packet::read(const int8_t* buf, unsigned int len, unsigned int& index) {
    return readPacketInfo(buf, len, index);
}

}
}
