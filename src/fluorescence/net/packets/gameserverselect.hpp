#ifndef FLUO_NET_PACKETS_GAMESERVERSELECT_HPP
#define FLUO_NET_PACKETS_GAMESERVERSELECT_HPP

#include <net/packet.hpp>

namespace fluo {
namespace net {

namespace packets {

class GameServerSelect : public Packet {
public:
    GameServerSelect(uint16_t serverIndex);

    virtual bool write(int8_t* buf, unsigned int len, unsigned int& index) const;

private:
    uint16_t serverIndex_;
};

}
}
}

#endif
