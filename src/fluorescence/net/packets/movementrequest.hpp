#ifndef FLUO_NET_PACKETS_MOVEREQUEST_HPP
#define FLUO_NET_PACKETS_MOVEREQUEST_HPP

#include <net/packet.hpp>

namespace fluo {
namespace net {

namespace packets {

class MovementRequest : public Packet {
public:
    MovementRequest(uint8_t direction, uint8_t sequence, uint32_t fastWalkKey);

    virtual bool write(int8_t* buf, unsigned int len, unsigned int& index) const;

private:
    uint8_t direction_;
    uint8_t sequence_;
    uint32_t fastWalkKey_;
};

}
}
}

#endif
