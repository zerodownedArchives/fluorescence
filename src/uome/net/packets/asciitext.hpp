#ifndef UOME_NET_PACKETS_ASCIITEXT_HPP
#define UOME_NET_PACKETS_ASCIITEXT_HPP

#include "basetext.hpp"

#include <typedefs.hpp>

namespace uome {
namespace net {

namespace packets {

class AsciiText : public BaseText {
public:
    AsciiText();

    virtual bool read(const int8_t* buf, unsigned int len, unsigned int& index);
};

}
}
}

#endif
