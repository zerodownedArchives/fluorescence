
#include "fastwalkinit.hpp"

namespace fluo {
namespace net {
namespace packets {
namespace bf {

FastWalkInit::FastWalkInit() : Packet(0x01) {
}

bool FastWalkInit::read(const int8_t* buf, unsigned int len, unsigned int& index) {
    bool ret = true;

    for (unsigned int i = 0; i < 6; ++i) {
        ret &= PacketReader::read(buf, len, index, keys_[i]);
    }

    return ret;
}

void FastWalkInit::onReceive() {
}

}
}
}
}
