
#include "loginreject.hpp"

#include <ui/manager.hpp>
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
    switch (reason_) {
    case 0x00:
        ui::Manager::getSingleton()->openMessageBox("Login denied: unknown user");
        break;
    case 0x01:
        ui::Manager::getSingleton()->openMessageBox("Login denied: account in use");
        break;
    case 0x02:
        ui::Manager::getSingleton()->openMessageBox("Login denied: account disabled");
        break;
    case 0x03:
        ui::Manager::getSingleton()->openMessageBox("Login denied: bad password");
        break;
    default:
        ui::Manager::getSingleton()->openMessageBox("Login denied: communication error");
        break;
    }

    net::Manager::getSingleton()->disconnect();
}

}
}
}
