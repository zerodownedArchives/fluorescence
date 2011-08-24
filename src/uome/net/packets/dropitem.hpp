#ifndef UOME_NET_PACKETS_DROPITEM_HPP
#define UOME_NET_PACKETS_DROPITEM_HPP

#include <boost/shared_ptr.hpp>

#include <typedefs.hpp>
#include <net/packet.hpp>

namespace uome {
namespace world {
    class DynamicItem;
}

namespace net {

namespace packets {

class DropItem : public Packet {
public:
    DropItem(const world::DynamicItem* itm, boost::shared_ptr<world::DynamicItem> target);
    DropItem(const world::DynamicItem* itm, unsigned int x, unsigned int y, int z);

    virtual bool write(int8_t* buf, unsigned int len, unsigned int& index) const;

private:
    Serial serial_;
    uint16_t locX_;
    uint16_t locY_;
    int8_t locZ_;
    Serial containerSerial_;
};

}
}
}

#endif
