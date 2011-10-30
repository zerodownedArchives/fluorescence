#ifndef FLUO_NET_PACKETS_DISPLAYCONTAINER_HPP
#define FLUO_NET_PACKETS_DISPLAYCONTAINER_HPP

#include <net/packet.hpp>

#include <typedefs.hpp>

namespace fluo {
namespace net {

namespace packets {

class DisplayContainer : public Packet {
public:
    DisplayContainer();

    virtual bool read(const int8_t* buf, unsigned int len, unsigned int& index);

    virtual void onReceive();


    Serial serial_;
    uint16_t gumpId_;
};

}
}
}

#endif
