
#include "loginreject.hpp"

#include <ui/gumpmenus.hpp>
#include <net/manager.hpp>

namespace uome {
namespace net {
namespace packets {

LoginReject::LoginReject() : Packet(0x82, 2) {
}


bool LoginReject::read(const int8_t* buf, unsigned int len, unsigned int& index) {
    bool ret = true;

    ret &= PacketReader::read(buf, len, index, reason_);

    return ret;
}

void LoginReject::onReceive() {
    UnicodeString reasonStr;
    switch (reason_) {
    case 0x00:
        reasonStr = "Login denied: unknown user";
        break;
    case 0x01:
        reasonStr = "Login denied: account in use";
        break;
    case 0x02:
        reasonStr = "Login denied: account disabled";
        break;
    case 0x03:
        reasonStr = "Login denied: bad password";
        break;
    default:
        reasonStr = "Login denied: communication error";
        break;
    }

    ui::GumpMenus::openMessageBox(reasonStr);

    net::Manager::getSingleton()->disconnect();
}

}
}
}
