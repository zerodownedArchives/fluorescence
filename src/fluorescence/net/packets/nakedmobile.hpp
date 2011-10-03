#ifndef FLUO_NET_PACKETS_NAKEDMOBILE_HPP
#define FLUO_NET_PACKETS_NAKEDMOBILE_HPP

#include <net/packet.hpp>

#include <typedefs.hpp>

namespace fluo {
namespace net {

namespace packets {

class NakedMobile : public Packet {
public:
    NakedMobile();

    virtual bool read(const int8_t* buf, unsigned int len, unsigned int& index);

    virtual void onReceive();


    Serial serial_;
    uint16_t bodyId_;
    uint16_t locX_;
    uint16_t locY_;
    int8_t locZ_;
    uint8_t direction_;
    uint16_t hue_;
    uint8_t status_;
    uint8_t notoriety_;
};

}
}
}

#endif
