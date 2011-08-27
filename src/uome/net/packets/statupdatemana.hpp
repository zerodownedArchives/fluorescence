#ifndef UOME_NET_PACKETS_STATUPDATEMANA_HPP
#define UOME_NET_PACKETS_STATUPDATEMANA_HPP

#include <typedefs.hpp>
#include <net/packet.hpp>

namespace uome {
namespace net {

namespace packets {

class StatUpdateMana : public Packet {
public:
    StatUpdateMana();

    virtual bool read(const int8_t* buf, unsigned int len, unsigned int& index);

    virtual void onReceive();

private:
    Serial serial_;
    uint16_t maximum_;
    uint16_t current_;
};

}
}
}

#endif
