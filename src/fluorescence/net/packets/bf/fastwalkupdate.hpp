#ifndef FLUO_NET_PACKETS_BF_FASTWALKUPDATE_HPP
#define FLUO_NET_PACKETS_BF__HPP

#include <net/packet.hpp>

namespace fluo {
namespace net {
namespace packets {
namespace bf {

class FastWalkUpdate : public Packet {
public:
    FastWalkUpdate();

    virtual bool read(const int8_t* buf, unsigned int len, unsigned int& index);

    virtual void onReceive();

private:
    uint32_t key_;
};

}
}
}
}

#endif
