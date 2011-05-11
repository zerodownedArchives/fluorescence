
#include "logincomplete.hpp"

#include <client.hpp>

namespace uome {
namespace net {
namespace packets {

LoginComplete::LoginComplete() : Packet(0x55, 1) {
}

bool LoginComplete::read(const int8_t* buf, unsigned int len, unsigned int& index) {
    return true;
}

void LoginComplete::onReceive() {
    Client::getSingleton()->loginComplete();
}

}
}
}
