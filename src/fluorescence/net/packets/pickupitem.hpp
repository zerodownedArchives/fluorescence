#ifndef FLUO_NET_PACKETS_PICKUPITEM_HPP
#define FLUO_NET_PACKETS_PICKUPITEM_HPP

#include <typedefs.hpp>
#include <net/packet.hpp>

namespace fluo {
namespace world {
    class DynamicItem;
}

namespace net {

namespace packets {

class PickUpItem : public Packet {
public:
    PickUpItem(const world::DynamicItem* itm, unsigned int amount);

    virtual bool write(int8_t* buf, unsigned int len, unsigned int& index) const;

private:
    Serial serial_;
    uint16_t amount_;
};

}
}
}

#endif
