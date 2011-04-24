
#include "manager.hpp"

#include <misc/logger.hpp>

namespace uome {
namespace net {

Manager* Manager::singleton_ = 0;

bool Manager::create(const boost::program_options::variables_map& config) {
    if (!singleton_) {
        try {
            singleton_ = new Manager(config);
        } catch (const std::exception& ex) {
            LOGARG_CRITICAL(LOGTYPE_NETWORK, "Error initializing world::Manager: %s", ex.what());
            return false;
        }
    }

    return true;
}

Manager* Manager::getSingleton() {
    if (!singleton_) {
        throw Exception("uome::net::Manager Singleton not created yet");
    }

    return singleton_;
}

Manager::Manager(const boost::program_options::variables_map& config) {
}

}
}
