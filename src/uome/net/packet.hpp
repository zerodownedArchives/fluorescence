#ifndef UOME_NET_PACKET_HPP
#define UOME_NET_PACKET_HPP

#include <stdint.h>

#include "packetwriter.hpp"
#include "packetreader.hpp"

namespace uome {
namespace net {

class Packet {
public:
    // 0 indicates a variable sized packet
    Packet(uint8_t id, uint16_t size = 0);

    virtual bool write(int8_t* buf, unsigned int len, unsigned int& index) const;
    virtual bool read(const int8_t* buf, unsigned int len, unsigned int& index);

    bool hasVariableSize();
    uint16_t getSize(); ///< for fixed size packets
    void setSize(uint16_t val);

    virtual void onReceive();

protected:
    virtual bool writePacketInfo(int8_t* buf, unsigned int len, unsigned int& index) const;

    uint8_t id_;
    uint16_t size_;

};

}
}

#endif
