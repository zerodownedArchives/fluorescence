#ifndef UOME_NET_PACKETS_CHARACTERLIST_HPP
#define UOME_NET_PACKETS_CHARACTERLIST_HPP

#include <net/packet.hpp>
#include <misc/string.hpp>

#include <vector>

namespace uome {
namespace net {

namespace packets {

class CharacterList : public Packet {
public:
    CharacterList();

    virtual bool read(const int8_t* buf, unsigned int len, unsigned int& index);

    virtual void onReceive();


    uint8_t charCount_;
    UnicodeString charNames_[6];
    UnicodeString charPasswords_[6];

    uint8_t cityCount_;
    std::vector<uint8_t> cityIndices_;
    std::vector<UnicodeString> cityNames_;
    std::vector<UnicodeString> tavernNames_;

    uint32_t flags_;
};

}
}
}

#endif
