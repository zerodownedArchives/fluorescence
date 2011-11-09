
#include "bf.hpp"

#include "bf/changemap.hpp"
#include "bf/opencontextmenu.hpp"
#include "bf/fastwalkinit.hpp"
#include "bf/fastwalkupdate.hpp"

#include <misc/log.hpp>

namespace fluo {
namespace net {
namespace packets {

BF::BF() : MultiPurposePacket(0xBF) {
}

BF::BF(boost::shared_ptr<Packet> subpacket) : MultiPurposePacket(0xBF, subpacket) {
}

boost::shared_ptr<Packet> BF::getSubpacket(uint16_t subId) {
    //LOG_DEBUG << "BF::getSubpacket for id " << (unsigned int)subId << std::endl;
    boost::shared_ptr<Packet> ret;

    switch (subId) {
    case 0x01: ret.reset(new bf::FastWalkInit()); break;
    case 0x02: ret.reset(new bf::FastWalkUpdate()); break;
    case 0x08: ret.reset(new bf::ChangeMap()); break;
    case 0x14: ret.reset(new bf::OpenContextMenu()); break;
    }

    return ret;
}

}
}
}
