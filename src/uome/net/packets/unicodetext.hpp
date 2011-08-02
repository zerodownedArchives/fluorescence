#ifndef UOME_NET_PACKETS_UNICODETEXT_HPP
#define UOME_NET_PACKETS_UNICODETEXT_HPP

#include <net/packet.hpp>

#include <typedefs.hpp>

namespace uome {
namespace net {

namespace packets {

class UnicodeText : public Packet {
public:
    UnicodeText();

    virtual bool read(const int8_t* buf, unsigned int len, unsigned int& index);

    virtual void onReceive();


    Serial serial_;
    uint16_t bodyId_;
    uint8_t type_;
    uint16_t hue_;
    uint16_t font_;
    int8_t language_[4];
    UnicodeString speaker_;
    UnicodeString text_;
};

}
}
}

#endif
