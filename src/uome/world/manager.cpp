
#include "manager.hpp"

#include "sectormanager.hpp"
#include "lightmanager.hpp"

#include <misc/exception.hpp>
#include <misc/log.hpp>

#include <net/packets/playerinit.hpp>
#include <net/packets/teleport.hpp>
#include <net/packets/nakedmobile.hpp>

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

void Manager::initPlayer(const net::packets::PlayerInit* packet) {
    player_.reset(new Mobile(packet->serial_));
    player_->setLocation(packet->locX_, packet->locY_, packet->locZ_);
    player_->setBodyId(packet->bodyId_);
    player_->setDirection(packet->direction_);

    mobiles_[packet->serial_] = player_;

    LOG_DEBUG << "Location after player init: " << player_->getLocX() << "/" << player_->getLocY() << "/" << (unsigned int)player_->getLocZ() << std::endl;
}

boost::shared_ptr<Mobile> Manager::getPlayer() {
    return player_;
}

void Manager::handleTeleport(const net::packets::Teleport* packet) {
    if (!player_) {
        LOG_ERROR << "Received teleport packet before player was initialized" << std::endl;
        return;
    }

    if (player_->getSerial() != packet->serial_) {
        LOG_ERROR << "Received teleport packet for wrong serial" << std::endl;
        return;
    }

    player_->setLocation(packet->locX_, packet->locY_, packet->locZ_);
    player_->setBodyId(packet->bodyId_);
    player_->setDirection(packet->direction_);
    player_->setHue(packet->hue_);

    // TODO: handle status
}

void Manager::handleNakedMobile(const net::packets::NakedMobile* packet) {
    boost::shared_ptr<Mobile> mob;

    // check if mobile exists already
    std::map<Serial, boost::shared_ptr<Mobile> >::iterator iter = mobiles_.find(packet->serial_);
    if (iter == mobiles_.end()) {
        mob.reset(new Mobile(packet->serial_));
        mobiles_[packet->serial_] = mob;
    } else {
        mob = iter->second;
    }

    mob->setLocation(packet->locX_, packet->locY_, packet->locZ_);
    mob->setBodyId(packet->bodyId_);
    mob->setDirection(packet->direction_);
    mob->setHue(packet->hue_);

    // TODO: handle status
}

}
}
