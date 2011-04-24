#ifndef UOME_NETWORK_PACKETS_LOGINREQUEST_HPP
#define UOME_NETWORK_PACKETS_LOGINREQUEST_HPP

#include <net/packet.hpp>

#include <misc/string.hpp>

namespace uome {
namespace net {
namespace packets {

class LoginRequest : public Packet {
public:
    LoginRequest(const UnicodeString& name, const UnicodeString& password);

    virtual bool write(int8_t* buf, unsigned int len, unsigned int& index) const;

private:
    UnicodeString name_;
    UnicodeString password_;
};

}
}
}

#endif
