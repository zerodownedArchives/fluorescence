
#include "serverredirect.hpp"

#include <net/manager.hpp>

#ifdef WIN32
#include <WinSock2.h>
#else
#include <netinet/in.h>
#endif

namespace fluo {
namespace net {
namespace packets {

ServerRedirect::ServerRedirect() : Packet(0x8C, 11) {
}


bool ServerRedirect::read(const int8_t* buf, unsigned int len, unsigned int& index) {
    bool ret = true;

    ret &= PacketReader::read(buf, len, index, ipaddr_);
    ret &= PacketReader::read(buf, len, index, port_);
    ret &= PacketReader::read(buf, len, index, encryptionKey_);

    // swap bytes again
    ipaddr_ = htonl(ipaddr_);

    return ret;
}

void ServerRedirect::onReceive() {
    net::Manager::getSingleton()->handleServerRedirect(this);
}

}
}
}
