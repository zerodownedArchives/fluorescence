#ifndef UOME_NET_PACKET_HPP
#define UOME_NET_PACKET_HPP

#include <stdint.h>

#include "packetwriter.hpp"
#include "packetreader.hpp"

namespace uome {
namespace net {

class Packet {
public:
    Packet(uint32_t id);

    virtual bool write(int8_t* buf, unsigned int len, unsigned int& index) const;
    virtual bool read(const int8_t* buf, unsigned int len, unsigned int& index);

protected:
    virtual bool writePacketInfo(int8_t* buf, unsigned int len, unsigned int& index) const;

    uint32_t id_;

};

}
}

#endif
