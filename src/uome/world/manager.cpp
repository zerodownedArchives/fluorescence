
#include "manager.hpp"

#include "sectormanager.hpp"

#include <misc/logger.hpp>

namespace uome {
namespace world {

Manager* Manager::singleton_ = NULL;

Manager* Manager::getSingleton() {
    return singleton_;
}

bool Manager::create(const boost::program_options::variables_map& config) {
    if (!singleton_) {
        try {
            singleton_ = new Manager(config);
        } catch (const std::exception& ex) {
            LOGARG_CRITICAL(LOGTYPE_WORLD, "Error initializing world::Manager: %s", ex.what());
            return false;
        }
    }

    return true;
}

Manager::Manager(const boost::program_options::variables_map& config) {
    sectorManager_.reset(new SectorManager(config));
}

boost::shared_ptr<SectorManager> Manager::getSectorManager() {
    return sectorManager_;
}

}
}
