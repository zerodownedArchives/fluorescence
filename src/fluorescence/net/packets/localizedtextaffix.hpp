#ifndef FLUO_NET_PACKETS_LOCALIZEDTEXTAFFIX_HPP
#define FLUO_NET_PACKETS_LOCALIZEDTEXTAFFIX_HPP

#include "basetext.hpp"

namespace fluo {
namespace net {

namespace packets {

class LocalizedTextAffix : public BaseText {
public:
    LocalizedTextAffix();

    virtual bool read(const int8_t* buf, unsigned int len, unsigned int& index);

private:
    uint32_t clilocId_;
    uint8_t affixType_;
    UnicodeString affix_;
    UnicodeString arguments_;
};

}
}
}

#endif
