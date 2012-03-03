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

#include "playerwalkmanager.hpp"

#include <boost/bind.hpp>

#include "manager.hpp"
#include "smoothmovement.hpp"
#include "smoothmovementmanager.hpp"
#include "mobile.hpp"
#include "sectormanager.hpp"
#include "sector.hpp"

#include <net/manager.hpp>
#include <net/walkpacketmanager.hpp>

#include <data/manager.hpp>
#include <data/maploader.hpp>

#include <misc/log.hpp>

namespace fluo {
namespace world {

PlayerWalkManager::PlayerWalkManager() : isWalking_(false), lastIsWalking_(false), millisToNextMove_(0) {
}
    
void PlayerWalkManager::setWalkDirection(uint8_t direction) {
    requestedDirection_ = direction;
    isWalking_ = true;
}

void PlayerWalkManager::stopAtNextTile() {
    isWalking_ = false;
}

void PlayerWalkManager::stopImmediately() {
    isWalking_ = false;
    millisToNextMove_ = 0;
    world::Manager::getSmoothMovementManager()->clear(world::Manager::getSingleton()->getPlayer()->getSerial());
}

void PlayerWalkManager::update(unsigned int elapsedMillis) {
    millisToNextMove_ -= elapsedMillis;
    
    if (!isWalking_) {
        lastIsWalking_ = false;
        // anim is ended automatically by smooth movement object
        return;
    }
    
    boost::shared_ptr<world::Mobile> player = world::Manager::getSingleton()->getPlayer();
    
    if (!lastIsWalking_ && player->getDirection() != requestedDirection_) {
        // just turn and wait for a bit
        millisToNextMove_ = 150;
        player->setDirection(requestedDirection_);
        net::Manager::getWalkPacketManager()->sendMovementRequest(requestedDirection_);
        lastIsWalking_ = false;
    } else if (millisToNextMove_ <= 0) {
        // starting to walk now => start anim
        if (!lastIsWalking_) {
            player->animate(player->getWalkAnim(), 0, AnimRepeatMode::LOOP);
        }

        if (player->getDirection() != requestedDirection_) {
            player->setDirection(requestedDirection_);
            net::Manager::getWalkPacketManager()->sendMovementRequest(requestedDirection_);
        }
        
        CL_Vec3f newLoc;
        bool movementPossible = checkMovement(player->getLocation(), requestedDirection_, newLoc);
        if (movementPossible) {
            // TODO: check running, mounted. different speed
            unsigned int moveDuration = 100;
            
            millisToNextMove_ = moveDuration;;
            
            world::SmoothMovement mov(player, newLoc, moveDuration);
            mov.setFinishedCallback(boost::bind(&PlayerWalkManager::onSmoothMovementFinish, this));
            world::Manager::getSmoothMovementManager()->add(player->getSerial(), mov);
            net::Manager::getWalkPacketManager()->sendMovementRequest(requestedDirection_);
            
            lastIsWalking_ = true;
        } else {
            player->stopAnim();
            lastIsWalking_ = false;
        }
    }
}

bool PlayerWalkManager::isWalking() const {
    return isWalking_;
}

void PlayerWalkManager::onSmoothMovementFinish() {
    if (!isWalking_) {
        boost::shared_ptr<world::Mobile> player = world::Manager::getSingleton()->getPlayer();
        player->stopAnim();
    }
}

bool PlayerWalkManager::checkMovement(const CL_Vec3f& curLoc, unsigned int direction, CL_Vec3f& outLoc) const {
    CL_Vec3f diff;
    CL_Vec3f curRound = CL_Vec3f(curLoc).round();
    
    switch (direction & 0x7) {
        case Direction::N: diff = CL_Vec3f(0, -1, 0); break;
        case Direction::NE: diff = CL_Vec3f(1, -1, 0); break;
        case Direction::E: diff = CL_Vec3f(1, 0, 0); break;
        case Direction::SE: diff = CL_Vec3f(1, 1, 0); break;
        case Direction::S: diff = CL_Vec3f(0, 1, 0); break;
        case Direction::SW: diff = CL_Vec3f(-1, 1, 0); break;
        case Direction::W: diff = CL_Vec3f(-1, 0, 0); break;
        case Direction::NW: diff = CL_Vec3f(-1, -1, 0); break;
    }
    
    outLoc = curRound + diff;
    
    // don't run out of bounds
    boost::shared_ptr<data::MapLoader> mapLoader = data::Manager::getMapLoader(world::Manager::getSingleton()->getCurrentMapId());
    if (!mapLoader || outLoc.x < 0 || outLoc.y < 0 || outLoc.x > mapLoader->getBlockCountX() * 8 || outLoc.y > mapLoader->getBlockCountY() * 8) {
        outLoc = curLoc;
        return false;
    }
    
    //bool checkDiagonals = (direction & 0x1) == 0x1;
    
    boost::shared_ptr<Sector> newSector = world::Manager::getSectorManager()->getSectorForCoordinates(outLoc.x, outLoc.y);
    bool sectorOkay = newSector->checkMovement(curRound, outLoc);
    
    sectorOkay = !sectorOkay;
    
    return true;
}

}
}

