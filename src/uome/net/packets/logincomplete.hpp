#ifndef UOME_NET_PACKETS_LOGINCOMPLETE_HPP
#define UOME_NET_PACKETS_LOGINCOMPLETE_HPP

#include <net/packet.hpp>

namespace uome {
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
