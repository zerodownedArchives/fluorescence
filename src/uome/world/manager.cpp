
#include "manager.hpp"

#include "sectormanager.hpp"
#include "lightmanager.hpp"

#include <misc/logger.hpp>

namespace uome {
namespace world {

Manager* Manager::singleton_ = NULL;

Manager* Manager::getSingleton() {
    if (!singleton_) {
        throw Exception("uome::world::Manager Singleton not created yet");
    }
    return singleton_;
}

bool Manager::create(const Config& config) {
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

void Manager::destroy() {
    if (singleton_) {
        delete singleton_;
        singleton_ = NULL;
    }
}

Manager::Manager(const Config& config) : currentMapId_(0) {
    sectorManager_.reset(new SectorManager(config));
    lightManager_.reset(new LightManager());
}

Manager::~Manager() {
    LOG_INFO(LOGTYPE_WORLD, "Manager shutdown");
}

boost::shared_ptr<SectorManager> Manager::getSectorManager() {
    return sectorManager_;
}

unsigned int Manager::getCurrentMapId() {
    return currentMapId_;
}

void Manager::setCurrentMapId(unsigned int id) {
    currentMapId_ = id;
}

boost::shared_ptr<LightManager> Manager::getLightManager() {
    return lightManager_;
}

}
}
