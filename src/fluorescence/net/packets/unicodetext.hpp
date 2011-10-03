#ifndef FLUO_NET_PACKETS_UNICODETEXT_HPP
#define FLUO_NET_PACKETS_UNICODETEXT_HPP

#include "basetext.hpp"

namespace fluo {
namespace net {

namespace packets {

class UnicodeText : public BaseText {
public:
    UnicodeText();

    virtual bool read(const int8_t* buf, unsigned int len, unsigned int& index);
};

}
}
}

#endif
