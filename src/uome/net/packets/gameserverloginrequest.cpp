
#include "gameserverloginrequest.hpp"

namespace uome {
namespace net {
namespace packets {

GameServerLoginRequest::GameServerLoginRequest(const UnicodeString& name, const UnicodeString& password, uint32_t key) : Packet(0x91, 65),
    name_(name), password_(password), key_(key) {
}

bool GameServerLoginRequest::write(int8_t* buf, unsigned int len, unsigned int& index) const {
    bool ret = true;

    ret = ret && writePacketId(buf, len, index);
    ret = ret && PacketWriter::write(buf, len, index, key_);
    ret = ret && PacketWriter::writeUtf8Fixed(buf, len, index, name_, 30);
    ret = ret && PacketWriter::writeUtf8Fixed(buf, len, index, password_, 30);

    return ret;
}

}
}
}
