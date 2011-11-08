#ifndef FLUO_NET_PACKETS_BF_CONTEXTMENUREPLY_HPP
#define FLUO_NET_PACKETS_BF_CONTEXTMENUREPLY_HPP

#include <net/packet.hpp>

#include <typedefs.hpp>

namespace fluo {
namespace net {
namespace packets {
namespace bf {

class ContextMenuReply : public Packet {
public:
    ContextMenuReply(Serial serial, uint16_t choice);

    virtual bool write(int8_t* buf, unsigned int len, unsigned int& index) const;

private:
    Serial serial_;
    uint16_t choice_;
};

}
}
}
}

#endif
