#ifndef UOME_NET_PACKETS_SERVERLIST_HPP
#define UOME_NET_PACKETS_SERVERLIST_HPP

#include <net/packet.hpp>

#include <list>

namespace uome {
namespace net {
namespace packets {

class ServerList : public Packet {
public:

    struct ServerListEntry {
        uint16_t index_;
        UnicodeString name_;
        uint8_t fullPercentage_;
        uint8_t timezoneBias_;
        uint32_t serverAddress_;
    };

    ServerList();

    virtual bool read(const int8_t* buf, unsigned int len, unsigned int& index);

    virtual void onReceive();

    uint8_t flags_;
    std::list<ServerListEntry> listEntries_;
};

}
}
}

#endif
