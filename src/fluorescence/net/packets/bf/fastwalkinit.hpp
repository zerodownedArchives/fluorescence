#ifndef FLUO_NET_PACKETS_BF_FASTWALKINIT_HPP
#define FLUO_NET_PACKETS_BF_FASTWALKINIT_HPP

#include <net/packet.hpp>

namespace fluo {
namespace net {
namespace packets {
namespace bf {

class FastWalkInit : public Packet {
public:
    FastWalkInit();

    virtual bool read(const int8_t* buf, unsigned int len, unsigned int& index);

    virtual void onReceive();

private:
    uint32_t keys_[6];

};

}
}
}
}

#endif
