#ifndef UOME_NET_ECRYPTION_HPP
#define UOME_NET_ECRYPTION_HPP

#include <stdint.h>

namespace uome {
namespace net {

class Encryption {
public:
    virtual void encrypt(int8_t* dst, int8_t* src, unsigned int length) = 0;
    virtual void decrypt(int8_t* dst, int8_t* src, unsigned int length) = 0;
};

}
}

#endif
