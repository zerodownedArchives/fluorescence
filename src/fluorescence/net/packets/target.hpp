#ifndef FLUO_NET_PACKETS_TARGET_HPP
#define FLUO_NET_PACKETS_TARGET_HPP

#include <boost/shared_ptr.hpp>

#include <net/packet.hpp>
#include <typedefs.hpp>

namespace fluo {

namespace world {
    class IngameObject;
}

namespace net {
namespace packets {

class Target : public Packet {
public:
    Target();
    Target(boost::shared_ptr<world::IngameObject> obj);

    virtual bool write(int8_t* buf, unsigned int len, unsigned int& index) const;
    virtual bool read(const int8_t* buf, unsigned int len, unsigned int& index);

    virtual void onReceive();


    uint8_t targetType_;
    Serial senderSerial_;
    uint8_t flags_;
    Serial targetSerial_;
    uint16_t locX_;
    uint16_t locY_;
    int16_t locZ_;
    uint16_t graphicId_;
};

}
}
}

#endif
