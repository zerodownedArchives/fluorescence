#ifndef FLUO_NET_PACKETS_TELEPORT_HPP
#define FLUO_NET_PACKETS_TELEPORT_HPP

#include <net/packet.hpp>

#include <typedefs.hpp>

namespace fluo {
namespace net {

namespace packets {

class Teleport : public Packet {
public:
    Teleport();

    virtual bool read(const int8_t* buf, unsigned int len, unsigned int& index);

    virtual void onReceive();

    Serial serial_;
    uint16_t bodyId_;
    uint16_t hue_;
    uint8_t status_;
    uint16_t locX_;
    uint16_t locY_;
    uint8_t direction_;
    int8_t locZ_;

};

}
}
}

#endif
