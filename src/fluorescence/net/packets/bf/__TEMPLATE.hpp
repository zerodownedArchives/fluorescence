#ifndef FLUO_NET_PACKETS_BF__HPP
#define FLUO_NET_PACKETS_BF__HPP

#include <net/packet.hpp>

namespace fluo {
namespace net {
namespace packets {
namespace bf {

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
}

#endif
