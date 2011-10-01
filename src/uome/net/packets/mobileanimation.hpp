#ifndef UOME_NET_PACKETS_MOBILEANIMATION_HPP
#define UOME_NET_PACKETS_MOBILEANIMATION_HPP

#include <net/packet.hpp>

#include <typedefs.hpp>

namespace uome {
namespace net {

namespace packets {

class MobileAnimation : public Packet {
public:
    MobileAnimation();

    virtual bool read(const int8_t* buf, unsigned int len, unsigned int& index);

    virtual void onReceive();


    Serial serial_;
    uint16_t actionId_;
    uint16_t frameCount_;
    uint16_t repeatTimes_;
    uint8_t forward_;
    uint8_t repeat_;
    uint8_t delay_;
};

}
}
}

#endif
