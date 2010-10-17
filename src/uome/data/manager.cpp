
#include "manager.hpp"

namespace uome {
namespace data {

Manager* Manager::singleton_= NULL;

Manager* Manager::getSingleton() {
    if (!singleton_) {
        singleton_ = new Manager();
    }

    return singleton_;
}

Manager::Manager() {
}

Manager::~Manager() {
}



}
}
