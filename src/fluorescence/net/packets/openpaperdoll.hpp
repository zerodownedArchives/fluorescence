#ifndef FLUO_NET_PACKETS_OPENPAPERDOLL_HPP
#define FLUO_NET_PACKETS_OPENPAPERDOLL_HPP

#include <net/packet.hpp>

#include <typedefs.hpp>

namespace fluo {
namespace net {

namespace packets {

class OpenPaperdoll : public Packet {
public:
    OpenPaperdoll();

    virtual bool read(const int8_t* buf, unsigned int len, unsigned int& index);

    virtual void onReceive();

private:
    Serial serial_;
    UnicodeString name_;
    uint8_t status_;

};

}
}
}

#endif
