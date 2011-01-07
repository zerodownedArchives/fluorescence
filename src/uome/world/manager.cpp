
#include "manager.hpp"

#include "sectormanager.hpp"

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

void Manager::destroy() {
    if (singleton_) {
        delete singleton_;
        singleton_ = NULL;
    }
}

Manager::Manager(const boost::program_options::variables_map& config) : currentMapId_(0) {
    sectorManager_.reset(new SectorManager(config));
}

Manager::~Manager() {
}

SectorManager* Manager::getSectorManager() {
    return sectorManager_.get();
}

unsigned int Manager::getCurrentMapId() {
    return currentMapId_;
}

void Manager::setCurrentMapId(unsigned int id) {
    currentMapId_ = id;
}

}
}
