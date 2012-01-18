#ifndef FLUO_NET_PACKETS__HPP
#define FLUO_NET_PACKETS__HPP

#include <net/packet.hpp>

namespace fluo {
namespace net {

namespace packets {

class CreateCharacter : public Packet {
public:
    CreateCharacter();

    virtual bool write(int8_t* buf, unsigned int len, unsigned int& index) const;
    

    UnicodeString name_;
    UnicodeString password_;
    bool female_;
    
    uint8_t strength_;
    uint8_t dexterity_;
    uint8_t intelligence_;
    
    uint8_t skill1Id_;
    uint8_t skill1Value_;
    uint8_t skill2Id_;
    uint8_t skill2Value_;
    uint8_t skill3Id_;
    uint8_t skill3Value_;
    
    uint16_t hue_;
    uint16_t hairStyle_;
    uint16_t hairHue_;
    uint16_t beardStyle_;
    uint16_t beardHue_;
    
    uint16_t startCity_;
    
    uint16_t slot_;
    
    uint32_t encryptionKey_;
    
    uint16_t shirtHue_;
    uint16_t pantsHue_;
};

}
}
}

#endif
