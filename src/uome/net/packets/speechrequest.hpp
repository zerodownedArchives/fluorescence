#ifndef UOME_NET_PACKETS_SPEECHREQUEST_HPP
#define UOME_NET_PACKETS_SPEECHREQUEST_HPP

#include <net/packet.hpp>

namespace uome {
namespace net {

namespace packets {

class SpeechRequest : public Packet {
public:
    SpeechRequest(const UnicodeString& text);

    virtual bool write(int8_t* buf, unsigned int len, unsigned int& index) const;

    uint8_t speechMode_;
    uint16_t hue_;
    uint16_t font_;
    int8_t language_[4];
    // TODO: keywords
    UnicodeString text_;
};

}
}
}

#endif
