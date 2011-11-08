#ifndef FLUO_NET_PACKETS_BF_CHANGEMAP_HPP
#define FLUO_NET_PACKETS_BF_CHANGEMAP_HPP

#include <net/packet.hpp>

namespace fluo {
namespace net {
namespace packets {
namespace bf {

class ChangeMap : public Packet {
public:
    ChangeMap();

    virtual bool read(const int8_t* buf, unsigned int len, unsigned int& index);

    virtual void onReceive();

private:
    uint8_t mapId_;

};

}
}
}
}

#endif
