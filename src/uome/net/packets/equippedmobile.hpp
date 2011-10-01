#ifndef UOME_NET_PACKETS_EQUIPPEDMOBILE_HPP
#define UOME_NET_PACKETS_EQUIPPEDMOBILE_HPP

#include <list>

#include <net/packet.hpp>

#include <typedefs.hpp>

namespace uome {
namespace net {

namespace packets {

class EquippedMobile : public Packet {
private:
    struct EquipInfo {
        Serial serial_;
        uint16_t artId_;
        uint8_t layer_;
        uint16_t hue_;
    };

public:
    EquippedMobile();

    virtual bool read(const int8_t* buf, unsigned int len, unsigned int& index);

    virtual void onReceive();


    Serial serial_;
    uint16_t bodyId_;
    uint16_t locX_;
    uint16_t locY_;
    int8_t locZ_;
    uint8_t direction_;
    uint16_t hue_;
    uint8_t status_;
    uint8_t notoriety_;

    std::list<EquipInfo> equipment_;
};

}
}
}

#endif
