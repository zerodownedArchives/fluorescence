#ifndef FLUO_NET_PACKETS_CHARACTERSELECT_HPP
#define FLUO_NET_PACKETS_CHARACTERSELECT_HPP

#include <net/packet.hpp>

namespace fluo {
namespace net {

namespace packets {

class CharacterSelect : public Packet {
public:
    CharacterSelect(const UnicodeString& name, const UnicodeString& password, uint32_t index, uint32_t seed);

    virtual bool write(int8_t* buf, unsigned int len, unsigned int& index) const;

private:
    UnicodeString charName_;
    UnicodeString charPassword_;
    uint32_t charIndex_;
    uint32_t seed_;
};

}
}
}

#endif
