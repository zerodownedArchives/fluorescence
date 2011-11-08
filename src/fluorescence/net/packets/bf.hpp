#ifndef FLUO_NET_PACKETS_BF_HPP
#define FLUO_NET_PACKETS_BF_HPP

#include <net/multipurposepacket.hpp>

namespace fluo {
namespace net {

namespace packets {

class BF : public MultiPurposePacket {
public:
    BF();
    BF(boost::shared_ptr<Packet> subpacket);

protected:
    virtual boost::shared_ptr<Packet> getSubpacket(uint16_t subId);

};

}
}
}

#endif
