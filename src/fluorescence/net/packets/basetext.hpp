#ifndef FLUO_NET_PACKETS_BASETEXT_HPP
#define FLUO_NET_PACKETS_BASETEXT_HPP

#include <typedefs.hpp>

#include <net/packet.hpp>

namespace fluo {
namespace net {

namespace packets {

class BaseText : public Packet {
public:
    BaseText(uint8_t id);

    virtual void onReceive();

protected:
    Serial serial_;
    uint16_t bodyId_;
    uint8_t type_;
    uint16_t hue_;
    uint16_t font_;
    int8_t language_[4];
    UnicodeString speaker_;
    UnicodeString text_;


private:
    bool mobileMessage();
    bool itemMessage();

};

}
}
}

#endif
