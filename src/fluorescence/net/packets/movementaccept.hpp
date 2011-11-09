#ifndef FLUO_NET_PACKETS_MOVEMENTACCEPT_HPP
#define FLUO_NET_PACKETS_MOVEMENTACCEPT_HPP

#include <net/packet.hpp>

namespace fluo {
namespace net {

namespace packets {

// also sent by the client to request a movement resync
class MovementAccept : public Packet {
public:
    MovementAccept();

    virtual bool write(int8_t* buf, unsigned int len, unsigned int& index) const;
    virtual bool read(const int8_t* buf, unsigned int len, unsigned int& index);

    virtual void onReceive();

private:
    uint8_t sequence_;
    uint8_t status_;

};

}
}
}

#endif
