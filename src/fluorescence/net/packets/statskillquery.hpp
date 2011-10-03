#ifndef FLUO_NET_PACKETS_STATSKILLQUERY_HPP
#define FLUO_NET_PACKETS_STATSKILLQUERY_HPP

#include <typedefs.hpp>

#include <net/packet.hpp>

namespace fluo {
namespace net {

namespace packets {

class StatSkillQuery : public Packet {
public:
    static const unsigned int QUERY_STATS = 0x4;
    static const unsigned int QUERY_SKILLS = 0x5;

    StatSkillQuery(Serial serial, uint8_t queryType = QUERY_STATS);

    virtual bool write(int8_t* buf, unsigned int len, unsigned int& index) const;

private:
    Serial serial_;
    uint8_t queryType_;
};

}
}
}

#endif
