#ifndef FLUO_NET_PACKETS_LOCALIZEDTEXT_HPP
#define FLUO_NET_PACKETS_LOCALIZEDTEXT_HPP

#include "basetext.hpp"

namespace fluo {
namespace net {

namespace packets {

class LocalizedText : public BaseText {
public:
    LocalizedText();

    virtual bool read(const int8_t* buf, unsigned int len, unsigned int& index);

private:
    uint32_t clilocId_;
    UnicodeString arguments_;
};

}
}
}

#endif
