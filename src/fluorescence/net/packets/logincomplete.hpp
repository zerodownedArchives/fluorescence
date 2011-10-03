#ifndef FLUO_NET_PACKETS_LOGINCOMPLETE_HPP
#define FLUO_NET_PACKETS_LOGINCOMPLETE_HPP

#include <net/packet.hpp>

namespace fluo {
namespace net {

namespace packets {

class LoginComplete : public Packet {
public:
    LoginComplete();

    virtual bool read(const int8_t* buf, unsigned int len, unsigned int& index);

    virtual void onReceive();

};

}
}
}

#endif
