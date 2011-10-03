#ifndef FLUO_NET_PACKETS_DELETEOBJECT_HPP
#define FLUO_NET_PACKETS_DELETEOBJECT_HPP

#include <net/packet.hpp>

#include <typedefs.hpp>

namespace fluo {
namespace net {

namespace packets {

class DeleteObject : public Packet {
public:
    DeleteObject();

    virtual bool read(const int8_t* buf, unsigned int len, unsigned int& index);

    virtual void onReceive();

    Serial serial_;
};

}
}
}

#endif
