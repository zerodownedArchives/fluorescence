#ifndef FLUO_NET_PACKETS_BF_CONTEXTMENUDATA_HPP
#define FLUO_NET_PACKETS_BF_CONTEXTMENUDATA_HPP

#include <net/packet.hpp>

#include <list>
#include <typedefs.hpp>

namespace fluo {
namespace net {
namespace packets {
namespace bf {

struct ContextMenuEntry {
    uint16_t entryId_;
    unsigned int clilocId_;
    uint16_t flags_;
    uint16_t hue_;
};

class OpenContextMenu : public Packet {
public:
    OpenContextMenu();

    virtual bool read(const int8_t* buf, unsigned int len, unsigned int& index);

    virtual void onReceive();

    Serial serial_;
    std::list<ContextMenuEntry> entries_;
};

}
}
}
}

#endif
