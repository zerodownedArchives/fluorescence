
#include "servertarget.hpp"

#include <net/manager.hpp>
#include <net/packets/target.hpp>

#include <misc/log.hpp>

namespace fluo {
namespace ui {
namespace targeting {

ServerTarget::ServerTarget(bool allowGround) : Target(allowGround) {
}

void ServerTarget::onTarget(boost::shared_ptr<world::IngameObject> obj) {
    net::packets::Target pkt(obj);
    net::Manager::getSingleton()->send(pkt);
}

void ServerTarget::onCancel() {
    net::packets::Target pkt;
    net::Manager::getSingleton()->send(pkt);
}

}
}
}
