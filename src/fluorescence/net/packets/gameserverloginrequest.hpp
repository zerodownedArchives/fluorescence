#ifndef FLUO_NET_PACKETS_GAMESERVERLOGINREQUEST_HPP
#define FLUO_NET_PACKETS_GAMESERVERLOGINREQUEST_HPP

#include <net/packet.hpp>

#include <misc/string.hpp>

namespace fluo {
namespace net {
namespace packets {

class GameServerLoginRequest : public Packet {
public:
    GameServerLoginRequest(const UnicodeString& name, const UnicodeString& password, unsigned int key);

    virtual bool write(int8_t* buf, unsigned int len, unsigned int& index) const;

private:
    UnicodeString name_;
    UnicodeString password_;
    uint32_t key_;
};

}
}
}

#endif
