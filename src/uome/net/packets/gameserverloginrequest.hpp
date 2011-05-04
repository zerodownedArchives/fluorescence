#ifndef UOME_NET_PACKETS_GAMESERVERLOGINREQUEST_HPP
#define UOME_NET_PACKETS_GAMESERVERLOGINREQUEST_HPP

#include <net/packet.hpp>

namespace uome {
namespace net {
namespace packets {

class GameServerLoginRequest : public Packet {
public:
    GameServerLoginRequest(const std::string& name, const std::string& password, unsigned int key);

    virtual bool write(int8_t* buf, unsigned int len, unsigned int& index) const;

private:
    std::string name_;
    std::string password_;
    uint32_t key_;
};

}
}
}

#endif
