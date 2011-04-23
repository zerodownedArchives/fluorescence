
#include "manager.hpp"

namespace uome {
namespace net {

Manager* Manager::singleton_ = 0;

Manager* Manager::getSingleton() {
    if (!singleton_) {
        singleton_ = new Manager();
    }

    return singleton_;
}

Manager::Manager() {
}

}
}
