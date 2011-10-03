#ifndef FLUO_NET_PACKETS_WORLDITEM_HPP
#define FLUO_NET_PACKETS_WORLDITEM_HPP

#include <net/packet.hpp>

#include <typedefs.hpp>

namespace fluo {
namespace net {

namespace packets {

class WorldItem : public Packet {
public:
    WorldItem();

    virtual bool read(const int8_t* buf, unsigned int len, unsigned int& index);

    virtual void onReceive();


    Serial serial_;
    uint16_t artId_;
    uint16_t amount_;
    uint8_t stackIdOffset_;
    uint16_t locX_;
    uint16_t locY_;
    int8_t locZ_;
    int8_t direction_;
    uint16_t hue_;
    uint8_t status_;
};

}
}
}

#endif
