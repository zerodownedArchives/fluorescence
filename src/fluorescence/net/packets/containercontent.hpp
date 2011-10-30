#ifndef FLUO_NET_PACKETS_CONTAINERCONTENT_HPP
#define FLUO_NET_PACKETS_CONTAINERCONTENT_HPP

#include <list>

#include <net/packet.hpp>

#include <typedefs.hpp>

namespace fluo {
namespace net {

namespace packets {

class ContainerContent : public Packet {
public:
    ContainerContent();

    virtual bool read(const int8_t* buf, unsigned int len, unsigned int& index);

    virtual void onReceive();


private:
    struct ContainerContentEntry {
        Serial serial_;
        uint16_t artId_;
        uint8_t stackIdOffset_;
        uint16_t amount_;
        uint16_t pxX_;
        uint16_t pxY_;
        Serial container_;
        uint16_t hue_;
    };

    std::list<ContainerContentEntry> entries_;
};

}
}
}

#endif
