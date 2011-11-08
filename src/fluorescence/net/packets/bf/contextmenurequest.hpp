#ifndef FLUO_NET_PACKETS_BF_CONTEXTMENUREQUEST_HPP
#define FLUO_NET_PACKETS_BF_CONTEXTMENUREQUEST_HPP

#include <net/packet.hpp>

#include <typedefs.hpp>

namespace fluo {
namespace net {
namespace packets {
namespace bf {

class ContextMenuRequest : public Packet {
public:
    ContextMenuRequest(Serial serial);

    virtual bool write(int8_t* buf, unsigned int len, unsigned int& index) const;

private:
    Serial serial_;
};

}
}
}
}

#endif
