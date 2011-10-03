#ifndef FLUO_NET_XORENCRYTPTION_HPP
#define FLUO_NET_XORENCRYTPTION_HPP

#include "encryption.hpp"

namespace fluo {
namespace net {

class XorEncryption : public Encryption {
public:
    XorEncryption(uint32_t seed, uint32_t key1, uint32_t key2);

    virtual void encrypt(int8_t* dst, const int8_t* src, unsigned int length);
    virtual void decrypt(int8_t* dst, const int8_t* src, unsigned int length);

private:
    uint32_t seed_;

    uint32_t baseKey1_;
    uint32_t baseKey2_;

    uint32_t key1_;
    uint32_t key2_;
};

}
}

#endif
