
#include "gameserverselect.hpp"

namespace fluo {
namespace net {
namespace packets {

GameServerSelect::GameServerSelect(uint16_t serverIndex) : Packet(0xA0, 3), serverIndex_(serverIndex) {
}

bool GameServerSelect::write(int8_t* buf, unsigned int len, unsigned int& index) const {
    bool ret = true;

    ret = ret && writePacketId(buf, len, index);
    ret = ret && PacketWriter::write(buf, len, index, serverIndex_);

    return ret;
}

}
}
}
