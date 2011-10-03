#ifndef FLUO_NET_PACKETS_PICKUPREJECT_HPP
#define FLUO_NET_PACKETS_PICKUPREJECT_HPP

#include <net/packet.hpp>

namespace fluo {
namespace net {

namespace packets {

class PickUpReject : public Packet {
public:
    PickUpReject();

    virtual bool read(const int8_t* buf, unsigned int len, unsigned int& index);

    virtual void onReceive();

private:
    uint8_t reason_;

};

}
}
}

#endif
