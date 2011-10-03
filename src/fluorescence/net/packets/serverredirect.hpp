#ifndef FLUO_NET_PACKETS_SERVERREDIRECT_HPP
#define FLUO_NET_PACKETS_SERVERREDIRECT_HPP

#include <net/packet.hpp>

namespace fluo {
namespace net {

namespace packets {

class ServerRedirect : public Packet {
public:
    ServerRedirect();

    virtual bool read(const int8_t* buf, unsigned int len, unsigned int& index);

    virtual void onReceive();

    uint32_t ipaddr_;
    uint16_t port_;
    uint32_t encryptionKey_;
};

}
}
}

#endif
