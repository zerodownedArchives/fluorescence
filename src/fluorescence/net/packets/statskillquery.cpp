
#include "statskillquery.hpp"

namespace fluo {
namespace net {
namespace packets {

StatSkillQuery::StatSkillQuery(Serial serial, uint8_t queryType) : Packet(0x34, 10), serial_(serial), queryType_(queryType) {
}

bool StatSkillQuery::write(int8_t* buf, unsigned int len, unsigned int& index) const {
    bool ret = true;

    ret &= writePacketId(buf, len, index);
    ret &= PacketWriter::write(buf, len, index, 0xEDEDEDEDU); // unknown
    ret &= PacketWriter::write(buf, len, index, queryType_);
    ret &= PacketWriter::write(buf, len, index, serial_);

    return ret;
}

}
}
}
