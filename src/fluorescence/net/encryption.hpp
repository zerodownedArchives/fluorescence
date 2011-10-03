#ifndef FLUO_NET_ECRYPTION_HPP
#define FLUO_NET_ECRYPTION_HPP

#include <stdint.h>

namespace fluo {
namespace net {

class Encryption {
public:
    virtual void encrypt(int8_t* dst, const int8_t* src, unsigned int length) = 0;
    virtual void decrypt(int8_t* dst, const int8_t* src, unsigned int length) = 0;
};

}
}

#endif
