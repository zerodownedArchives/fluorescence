
#include "clientversion.hpp"

#include <net/manager.hpp>

#include <client.hpp>
#include <misc/config.hpp>
#include <misc/log.hpp>

namespace fluo {
namespace net {
namespace packets {

ClientVersion::ClientVersion() : Packet(0xBD) {
}

ClientVersion::ClientVersion(const UnicodeString& version) : Packet(0xBD), version_(version) {
}

bool ClientVersion::write(int8_t* buf, unsigned int len, unsigned int& index) const {
    bool ret = true;

    ret &= writePacketId(buf, len, index);
    unsigned int sizeIndex = preparePacketSize(index);

    ret &= PacketWriter::writeUtf8Null(buf, len, index, version_);

    writePacketSize(buf, len, index, sizeIndex);

    return ret;
}

bool ClientVersion::read(const int8_t* buf, unsigned int len, unsigned int& index) {
    // nothing to read here
    return true;
}

void ClientVersion::onReceive() {
    UnicodeString version = Client::getSingleton()->getConfig()["/fluo/shard/client@version-id"].asString();
    LOG_INFO << "Identifying myself as " << version << std::endl;
    ClientVersion reply(version);
    net::Manager::getSingleton()->send(reply);
}

}
}
}
