#ifndef UOME_NET_PACKETS_LOGINREQUEST_HPP
#define UOME_NET_PACKETS_LOGINREQUEST_HPP

#include <net/packet.hpp>

#include <misc/string.hpp>

namespace uome {
namespace net {
namespace packets {

class LoginRequest : public Packet {
public:
    LoginRequest(const std::string& name, const std::string& password);

    virtual bool write(int8_t* buf, unsigned int len, unsigned int& index) const;

private:
    std::string name_;
    std::string password_;
};

}
}
}

#endif
