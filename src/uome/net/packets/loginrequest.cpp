
#include "loginrequest.hpp"

namespace uome {
namespace net {
namespace packets {

LoginRequest::LoginRequest(const std::string& name, const std::string& password) : Packet(0x80, 62),
    name_(name), password_(password) {
}

bool LoginRequest::write(int8_t* buf, unsigned int len, unsigned int& index) const {
    bool ret = true;

    ret = ret && writePacketInfo(buf, len, index);
    ret = ret && PacketWriter::writeUtf8Fixed(buf, len, index, name_, 30);
    ret = ret && PacketWriter::writeUtf8Fixed(buf, len, index, password_, 30);
    ret = ret && PacketWriter::write(buf, len, index, (uint8_t)0);

    return ret;
}

}
}
}
