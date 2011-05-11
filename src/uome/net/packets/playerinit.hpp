#ifndef UOME_NET_PACKETS_PLAYERINIT_HPP
#define UOME_NET_PACKETS_PLAYERINIT_HPP

#include <net/packet.hpp>

#include <world/serverobject.hpp>

namespace uome {
namespace net {

namespace packets {

class PlayerInit : public Packet {
public:
    PlayerInit();

    virtual bool read(const int8_t* buf, unsigned int len, unsigned int& index);

    virtual void onReceive();


    world::Serial serial_;
    uint16_t bodyId_;
    uint16_t locX_;
    uint16_t locY_;
    int8_t locZ_;
    uint8_t direction_;
};

}
}
}

#endif
