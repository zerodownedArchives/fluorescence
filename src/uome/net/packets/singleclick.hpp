#ifndef UOME_NET_PACKETS_SINGLECLICK_HPP
#define UOME_NET_PACKETS_SINGLECLICK_HPP

#include <net/packet.hpp>

#include <typedefs.hpp>

namespace uome {
namespace net {

namespace packets {

class SingleClick : public Packet {
public:
    SingleClick(Serial serial);

    virtual bool write(int8_t* buf, unsigned int len, unsigned int& index) const;

private:
    Serial serial_;

};

}
}
}

#endif
