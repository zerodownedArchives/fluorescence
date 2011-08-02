#ifndef UOME_NET_PACKETS__HPP
#define UOME_NET_PACKETS__HPP

#include <net/packet.hpp>

namespace uome {
namespace net {

namespace packets {

class TEMPLATE : public Packet {
public:
    TEMPLATE();

    virtual bool write(int8_t* buf, unsigned int len, unsigned int& index) const;
    virtual bool read(const int8_t* buf, unsigned int len, unsigned int& index);

    virtual void onReceive();

};

}
}
}

#endif
