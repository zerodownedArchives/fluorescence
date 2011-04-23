
#include "noencryption.hpp"

#include <string.h>

namespace uome {
namespace net {

void NoEncryption::encrypt(int8_t* dst, const int8_t* src, unsigned int length) {
    memcpy(dst, src, length);
}

void NoEncryption::decrypt(int8_t* dst, const int8_t* src, unsigned int length) {
    memcpy(dst, src, length);
}

}
}
