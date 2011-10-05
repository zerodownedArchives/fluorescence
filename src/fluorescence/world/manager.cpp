
#include "manager.hpp"

#include "sectormanager.hpp"
#include "lightmanager.hpp"

#include <misc/exception.hpp>
#include <misc/log.hpp>

namespace fluo {
namespace world {

Manager* Manager::singleton_ = NULL;

Manager* Manager::getSingleton() {
    if (!singleton_) {
        throw Exception("fluo::world::Manager Singleton not created yet");
    }
    return singleton_;
}

bool Manager::create(const Config& config) {
    if (!singleton_) {
        try {
            singleton_ = new Manager(config);
        } catch (const std::exception& ex) {
            LOG_EMERGENCY << "Error initializing world::Manager: " << ex.what() << std::endl;
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
    LOG_INFO << "world::Manager shutdown" << std::endl;
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

boost::shared_ptr<Mobile> Manager::initPlayer(Serial serial) {
    player_.reset(new Mobile(serial));
    mobiles_[serial] = player_;

    return player_;
}

boost::shared_ptr<Mobile> Manager::getPlayer() {
    return player_;
}

void Manager::deleteObject(Serial serial) {
    boost::shared_ptr<Mobile> mob = getMobile(serial, false);
    if (mob) {
        mob->onDelete(mob);
        mobiles_.erase(serial);
    } else {
        boost::shared_ptr<DynamicItem> itm = getDynamicItem(serial, false);
        if (itm) {
            itm->onDelete(itm);
            dynamicItems_.erase(serial);
        }
    }
}

boost::shared_ptr<Mobile> Manager::getMobile(Serial serial, bool createIfNotExists) {
    boost::shared_ptr<Mobile> itm;

    std::map<Serial, boost::shared_ptr<Mobile> >::iterator iter = mobiles_.find(serial);
    if (iter != mobiles_.end()) {
        itm = iter->second;
    } else if (createIfNotExists) {
        itm.reset(new Mobile(serial));
        mobiles_[serial] = itm;
    }

    return itm;
}

boost::shared_ptr<DynamicItem> Manager::getDynamicItem(Serial serial, bool createIfNotExists) {
    boost::shared_ptr<DynamicItem> itm;

    std::map<Serial, boost::shared_ptr<DynamicItem> >::iterator iter = dynamicItems_.find(serial);
    if (iter != dynamicItems_.end()) {
        itm = iter->second;
    } else if(createIfNotExists) {
        itm.reset(new DynamicItem(serial));
        dynamicItems_[serial] = itm;
    }

    return itm;
}

void Manager::printItemSerials() {
    LOG_DEBUG << "all item serials:" << std::endl;

    std::map<Serial, boost::shared_ptr<DynamicItem> >::iterator iter = dynamicItems_.begin();
    std::map<Serial, boost::shared_ptr<DynamicItem> >::iterator end = dynamicItems_.end();

    for (; iter != end; ++iter) {
        LOG_DEBUG << "\titm serial=" << iter->second->getSerial() << std::endl;
    }
}


}
}