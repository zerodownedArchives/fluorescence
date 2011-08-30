#ifndef UOME_NET_PACKETS_STATUPDATESTAMINA_HPP
#define UOME_NET_PACKETS_STATUPDATESTAMINA_HPP

#include <typedefs.hpp>
#include <net/packet.hpp>

namespace uome {
namespace net {

namespace packets {

class StatUpdateStamina : public Packet {
public:
    StatUpdateStamina();

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
