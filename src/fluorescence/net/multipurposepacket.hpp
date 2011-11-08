#ifndef FLUO_NET_PACKETS_MULTIPURPOSEPACKET_HPP
#define FLUO_NET_PACKETS_MULTIPURPOSEPACKET_HPP

#include <boost/shared_ptr.hpp>

#include <net/packet.hpp>

namespace fluo {
namespace net {

namespace packets {

class MultiPurposePacket : public Packet {
public:
    MultiPurposePacket(uint8_t packetId);
    MultiPurposePacket(uint8_t packetId, boost::shared_ptr<Packet> subpacket);

    virtual bool write(int8_t* buf, unsigned int len, unsigned int& index) const;
    virtual bool read(const int8_t* buf, unsigned int len, unsigned int& index);

    virtual void onReceive();

protected:
    virtual boost::shared_ptr<Packet> getSubpacket(uint16_t subId) = 0;

private:
    boost::shared_ptr<Packet> subpacket_;

};

}
}
}

#endif
