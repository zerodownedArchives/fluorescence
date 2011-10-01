#ifndef UOME_NET_PACKETS_EQUIPPEDITEM_HPP
#define UOME_NET_PACKETS_EQUIPPEDITEM_HPP

#include <net/packet.hpp>

#include <typedefs.hpp>

namespace uome {
namespace net {

namespace packets {

class EquippedItem : public Packet {
public:
    EquippedItem();

    virtual bool read(const int8_t* buf, unsigned int len, unsigned int& index);

    virtual void onReceive();


    Serial serial_;
    uint16_t artId_;
    uint8_t layer_;
    Serial mobile_;
    uint16_t hue_;
};

}
}
}

#endif
