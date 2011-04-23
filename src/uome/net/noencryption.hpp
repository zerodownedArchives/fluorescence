#ifndef UOME_NET_NOENCRYTPTION_HPP
#define UOME_NET_NOENCRYTPTION_HPP

#include "encryption.hpp"

namespace uome {
namespace net {

class NoEncryption : public Encryption {
public:
    virtual void encrypt(int8_t* dst, const int8_t* src, unsigned int length);
    virtual void decrypt(int8_t* dst, const int8_t* src, unsigned int length);
};

}
}

#endif
