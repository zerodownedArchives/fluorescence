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
#include "effect.hpp"
#include "syslog.hpp"

#include <ui/manager.hpp>
#include <ui/cliprectmanager.hpp>

#include <net/manager.hpp>
#include <net/packets/34_statskillquery.hpp>

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

    sysLog_.reset(new SysLog());

    setAutoDeleteRange(18);
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

boost::shared_ptr<SysLog> Manager::getSysLog() {
    return getSingleton()->sysLog_;
}


unsigned int Manager::getCurrentMapId() {
    return currentMapId_;
}

void Manager::setCurrentMapId(unsigned int id) {
    if (currentMapId_ != id) {
        currentMapId_ = id;
        sectorManager_->onMapChange();
        if (player_) {
            player_->onLocationChanged(player_->getLocation()); // force sector change
        }
    }
}

boost::shared_ptr<LightManager> Manager::getLightManager() {
    return getSingleton()->lightManager_;
}

boost::shared_ptr<Mobile> Manager::initPlayer(Serial serial) {
    player_.reset(new Mobile(serial));
    mobiles_[serial] = player_;

    net::packets::StatSkillQuery queryPacket(player_->getSerial(), net::packets::StatSkillQuery::QUERY_SKILLS);
    net::Manager::getSingleton()->send(queryPacket);

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
    sectorManager_->updateSectorList();
    sysLog_->update(elapsedMillis);

    update(elapsedMillis);
}

void Manager::update(unsigned int elapsedMillis) {
    smoothMovementManager_->update(elapsedMillis);

    int playerX = player_->getLocXGame();
    int playerY = player_->getLocYGame();

    std::list<Serial> outOfRangeDelete;

    std::map<Serial, boost::shared_ptr<Mobile> >::iterator mobIter = mobiles_.begin();
    std::map<Serial, boost::shared_ptr<Mobile> >::iterator mobEnd = mobiles_.end();

    for (; mobIter != mobEnd; ++mobIter) {
        if (abs((int)mobIter->second->getLocXGame() - playerX) > autoDeleteRange_ ||
                abs((int)mobIter->second->getLocYGame() - playerY) > autoDeleteRange_) {
            outOfRangeDelete.push_back(mobIter->first);
        } else {
            updateObject(mobIter->second.get(), elapsedMillis);
        }
    }

    std::map<Serial, boost::shared_ptr<DynamicItem> >::iterator itmIter = dynamicItems_.begin();
    std::map<Serial, boost::shared_ptr<DynamicItem> >::iterator itmEnd = dynamicItems_.end();

    for (; itmIter != itmEnd; ++itmIter) {
        if ((abs((int)itmIter->second->getLocXGame() - playerX) > autoDeleteRange_ ||
                abs((int)itmIter->second->getLocYGame() - playerY) > autoDeleteRange_) && !itmIter->second->hasParent()) {
            outOfRangeDelete.push_back(itmIter->first);
        } else {
            updateObject(itmIter->second.get(), elapsedMillis);
        }
    }

    std::list<boost::shared_ptr<OverheadMessage> >::iterator msgIter = overheadMessages_.begin();
    std::list<boost::shared_ptr<OverheadMessage> >::iterator msgEnd = overheadMessages_.end();
    std::list<boost::shared_ptr<OverheadMessage> > expiredMessages;

    for (; msgIter != msgEnd; ++msgIter) {
        updateObject(msgIter->get(), elapsedMillis);

        if ((*msgIter)->isExpired()) {
            expiredMessages.push_back(*msgIter);
        }
    }

    if (!expiredMessages.empty()) {
        msgIter = expiredMessages.begin();
        msgEnd = expiredMessages.end();

        for (; msgIter != msgEnd; ++msgIter) {
            (*msgIter)->repaintRectangle(false);
            (*msgIter)->expire();
        }
    }


    std::list<boost::shared_ptr<Effect> >::iterator effectIter = effects_.begin();
    std::list<boost::shared_ptr<Effect> >::iterator effectEnd = effects_.end();
    std::list<boost::shared_ptr<Effect> > expiredEffects;

    for (; effectIter != effectEnd; ++effectIter) {
        (*effectIter)->update(elapsedMillis); // update effect itself (e.g. position)
        updateObject(effectIter->get(), elapsedMillis);

        if ((*effectIter)->isExpired()) {
            expiredEffects.push_back(*effectIter);
        }
    }

    if (!expiredEffects.empty()) {
        effectIter = expiredEffects.begin();
        effectEnd = expiredEffects.end();

        for (; effectIter != effectEnd; ++effectIter) {
            (*effectIter)->repaintRectangle(true);
            (*effectIter)->onDelete();
            effects_.remove(*effectIter);
        }
    }


    if (!outOfRangeDelete.empty()) {
        //LOG_DEBUG << "out of range coords=" << playerX << "/" << playerY << std::endl;
        std::list<Serial>::const_iterator iter = outOfRangeDelete.begin();
        std::list<Serial>::const_iterator end = outOfRangeDelete.end();

        for (; iter != end; ++iter) {
            deleteObject(*iter);
        }
    }

    sectorManager_->update(elapsedMillis);

    // do after sector sorting
    playerWalkManager_->update(elapsedMillis);
}

void Manager::updateObject(IngameObject* obj, unsigned int elapsedMillis) {
    obj->updateRenderData(elapsedMillis);
    bool depthUpdate = obj->getWorldRenderData().renderDepthUpdated();
    bool texOrVertUpdate = obj->getWorldRenderData().textureOrVerticesUpdated();
    if (depthUpdate || texOrVertUpdate) {
        // add previous and current vertex coordinates to clipped update range
        obj->repaintRectangle(true);
    }
}

void Manager::registerOverheadMessage(boost::shared_ptr<OverheadMessage> msg) {
    overheadMessages_.push_back(msg);
}

void Manager::unregisterOverheadMessage(boost::shared_ptr<OverheadMessage> msg) {
    overheadMessages_.remove(msg);
}

void Manager::setAutoDeleteRange(unsigned int range) {
    autoDeleteRange_ = range + 2;
}

void Manager::addEffect(boost::shared_ptr<Effect> effect) {
    effects_.push_back(effect);
}

void Manager::systemMessage(const UnicodeString& msg, unsigned int hue, unsigned int font) {
    LOG_INFO << "SysMsg: " << msg << std::endl;

    sysLog_->add(msg, hue, font);

    // TODO: add to journal
}

std::list<boost::shared_ptr<world::Effect> >::iterator Manager::effectsBegin() {
    return effects_.begin();
}

std::list<boost::shared_ptr<world::Effect> >::iterator Manager::effectsEnd() {
    return effects_.end();
}

void Manager::clear() {
    player_.reset();
    mobiles_.clear();
    dynamicItems_.clear();
    smoothMovementManager_->clear();
    overheadMessages_.clear();
    effects_.clear();
    sectorManager_->clear();
}

}
}
