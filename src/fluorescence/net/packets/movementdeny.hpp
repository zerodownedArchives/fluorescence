#ifndef FLUO_NET_PACKETS_MOVEMENTDENY_HPP
#define FLUO_NET_PACKETS__HPP

#include <net/packet.hpp>

namespace fluo {
namespace net {

namespace packets {

class MovementDeny : public Packet {
public:
    MovementDeny();

    virtual bool read(const int8_t* buf, unsigned int len, unsigned int& index);

    virtual void onReceive();

private:
    uint8_t sequence_;
    uint16_t locX_;
    uint16_t locY_;
    uint8_t direction_;
    int8_t locZ_;
};

}
}
}

#endif
