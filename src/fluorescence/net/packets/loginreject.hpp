#ifndef FLUO_NET_PACKETS_LOGINREJECT_HPP
#define FLUO_NET_PACKETS_LOGINREJECT_HPP

#include <net/packet.hpp>

namespace fluo {
namespace net {

namespace packets {

class LoginReject : public Packet {
public:
    LoginReject();

    virtual bool read(const int8_t* buf, unsigned int len, unsigned int& index);

    virtual void onReceive();

private:
    uint8_t reason_;
};

}
}
}

#endif
