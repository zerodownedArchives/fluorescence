
#include "packet.hpp"

#include <misc/log.hpp>

namespace fluo {
namespace net {

Packet::Packet(uint8_t id, uint16_t size) : id_(id), size_(size) {
}

bool Packet::writePacketId(int8_t* buf, unsigned int len, unsigned int& index) const {
    return PacketWriter::write(buf, len, index, id_);
}

bool Packet::writeSubPacketId(int8_t* buf, unsigned int len, unsigned int& index) const {
    uint16_t id16 = id_;
    return PacketWriter::write(buf, len, index, id16);
}

bool Packet::write(int8_t* buf, unsigned int len, unsigned int& index) const {
    return writePacketId(buf, len, index);
}

bool Packet::read(const int8_t* buf, unsigned int len, unsigned int& index) {
    return true;
}

bool Packet::hasVariableSize() {
    return size_ == 0;
}

void Packet::setSize(uint16_t val) {
    size_ = val;
}

uint16_t Packet::getSize() {
    return size_;
}

void Packet::onReceive() {
    LOG_ERROR << "Packet handler not implemented: " << std::hex << (unsigned int)id_ << std::dec << std::endl;
}

uint8_t Packet::getId() {
    return id_;
}

unsigned int Packet::preparePacketSize(unsigned int& index) const {
    index += 2;
    return index;
}

bool Packet::writePacketSize(int8_t* buf, unsigned int len, unsigned int& index, unsigned int sizeOffset) const {
    uint16_t dataLen = index - sizeOffset;
    //LOG_DEBUG << "writePacketSize: index=" << index << " sizeOffset=" << sizeOffset << " dataLen=" << dataLen << std::endl;
    dataLen += 3; // add id and len
    unsigned int tmpIndex = sizeOffset - 2;
    return PacketWriter::write(buf, len, tmpIndex, dataLen);
}

}
}
