/*
 * fluorescence is a free, customizable Ultima Online client.
 * Copyright (C) 2011-2012, http://fluorescence-client.org

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */



#include "manager.hpp"

#include "sectormanager.hpp"
#include "lightmanager.hpp"
#include "smoothmovementmanager.hpp"
#include "mobile.hpp"
#include "dynamicitem.hpp"
#include "overheadmessage.hpp"
#include "playerwalkmanager.hpp"

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
    smoothMovementManager_.reset(new SmoothMovementManager());
    playerWalkManager_.reset(new PlayerWalkManager());
}

Manager::~Manager() {
    LOG_INFO << "world::Manager shutdown" << std::endl;
}

boost::shared_ptr<SectorManager> Manager::getSectorManager() {
    return getSingleton()->sectorManager_;
}

boost::shared_ptr<SmoothMovementManager> Manager::getSmoothMovementManager() {
    return getSingleton()->smoothMovementManager_;
}

boost::shared_ptr<PlayerWalkManager> Manager::getPlayerWalkManager() {
    return getSingleton()->playerWalkManager_;
}

unsigned int Manager::getCurrentMapId() {
    return currentMapId_;
}

void Manager::setCurrentMapId(unsigned int id) {
    currentMapId_ = id;
}

boost::shared_ptr<LightManager> Manager::getLightManager() {
    return getSingleton()->lightManager_;
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
        mob->onDelete();
        mobiles_.erase(serial);
    } else {
        boost::shared_ptr<DynamicItem> itm = getDynamicItem(serial, false);
        if (itm) {
            itm->onDelete();
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

void Manager::step(unsigned int elapsedMillis) {
    world::Manager::getSectorManager()->deleteSectors();
    world::Manager::getSectorManager()->addNewSectors();

    update(elapsedMillis);
}

void Manager::update(unsigned int elapsedMillis) {
    playerWalkManager_->update(elapsedMillis);
    smoothMovementManager_->update(elapsedMillis);

    std::map<Serial, boost::shared_ptr<Mobile> >::iterator mobIter = mobiles_.begin();
    std::map<Serial, boost::shared_ptr<Mobile> >::iterator mobEnd = mobiles_.end();

    for (; mobIter != mobEnd; ++mobIter) {
        mobIter->second->updateRenderData(elapsedMillis);
    }

    std::map<Serial, boost::shared_ptr<DynamicItem> >::iterator itmIter = dynamicItems_.begin();
    std::map<Serial, boost::shared_ptr<DynamicItem> >::iterator itmEnd = dynamicItems_.end();

    for (; itmIter != itmEnd; ++itmIter) {
        itmIter->second->updateRenderData(elapsedMillis);
    }

    sectorManager_->update(elapsedMillis);

    std::list<boost::shared_ptr<OverheadMessage> >::iterator msgIter = overheadMessages_.begin();
    std::list<boost::shared_ptr<OverheadMessage> >::iterator msgEnd = overheadMessages_.end();
    std::list<boost::shared_ptr<OverheadMessage> > expiredMessages;

    for (; msgIter != msgEnd; ++msgIter) {
        (*msgIter)->updateRenderData(elapsedMillis);

        if ((*msgIter)->isExpired()) {
            expiredMessages.push_back(*msgIter);
        }
    }

    if (!expiredMessages.empty()) {
        msgIter = expiredMessages.begin();
        msgEnd = expiredMessages.end();

        for (; msgIter != msgEnd; ++msgIter) {
            (*msgIter)->expire();
        }
    }
}

void Manager::registerOverheadMessage(boost::shared_ptr<OverheadMessage> msg) {
    overheadMessages_.push_back(msg);
}

void Manager::unregisterOverheadMessage(boost::shared_ptr<OverheadMessage> msg) {
    overheadMessages_.remove(msg);
}

}
}
