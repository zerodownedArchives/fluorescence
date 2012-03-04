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

        CL_Vec3f newLoc;
        uint8_t curDirection = requestedDirection_;
        bool movementPossible = checkMovement(player->getLocation(), curDirection, newLoc);
        if (movementPossible) {
            if (player->getDirection() != curDirection) {
                player->setDirection(curDirection);
                net::Manager::getWalkPacketManager()->sendMovementRequest(curDirection);
            }
        
            unsigned int moveDuration;
            if (player->isMounted()) {
                if (curDirection & Direction::RUNNING) {
                    moveDuration = 95;
                } else {
                    moveDuration = 185;
                }
            } else {
                if (curDirection & Direction::RUNNING) {
                    moveDuration = 175;
                } else {
                    moveDuration = 375;
                }
            }
            
            millisToNextMove_ = moveDuration;;
            
            world::SmoothMovement mov(player, newLoc, moveDuration);
            mov.setFinishedCallback(boost::bind(&PlayerWalkManager::onSmoothMovementFinish, this));
            world::Manager::getSmoothMovementManager()->add(player->getSerial(), mov);
            net::Manager::getWalkPacketManager()->sendMovementRequest(curDirection);
            
            if (!lastIsWalking_) {
                player->animate(player->getWalkAnim(), 0, AnimRepeatMode::LOOP);
            }
            
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

bool PlayerWalkManager::checkMovement(const CL_Vec3f& curLoc, uint8_t& direction, CL_Vec3f& outLoc) const {
    CL_Vec3f diff = getDirectionOffset(direction);
    CL_Vec3f curRound = CL_Vec3f(curLoc).round();
    
    outLoc = curRound + diff;
    
    // don't run out of bounds
    boost::shared_ptr<data::MapLoader> mapLoader = data::Manager::getMapLoader(world::Manager::getSingleton()->getCurrentMapId());
    if (!mapLoader || outLoc.x < 0 || outLoc.y < 0 || outLoc.x > mapLoader->getBlockCountX() * 8 || outLoc.y > mapLoader->getBlockCountY() * 8) {
        outLoc = curLoc;
        return false;
    }
    
    boost::shared_ptr<Sector> curSector = world::Manager::getSectorManager()->getSectorForCoordinates(curRound.x, curRound.y);
    int stepReach = curSector->getStepReach(curRound);
    
    bool checkDiagonals = (direction & 0x1) == 0x1;
    
    boost::shared_ptr<Sector> newSector = world::Manager::getSectorManager()->getSectorForCoordinates(outLoc.x, outLoc.y);
    bool sectorOkay = newSector->checkMovement(curRound, stepReach, outLoc);
    
    if (checkDiagonals) {
        if (sectorOkay) {
            // check if the way is free on both sides
            unsigned int plusDirection = (direction & ~0x7) | ((direction + 1) & 0x7);
            diff = getDirectionOffset(plusDirection);
            CL_Vec3f plusLoc = curRound + diff;
            newSector = world::Manager::getSectorManager()->getSectorForCoordinates(plusLoc.x, plusLoc.y);
            bool plusOkay = newSector->checkMovement(curRound, stepReach, plusLoc);
            
            unsigned int minusDirection = (direction & ~0x7) | ((direction - 1) & 0x7);
            diff = getDirectionOffset(minusDirection);
            CL_Vec3f minusLoc = curRound + diff;
            newSector = world::Manager::getSectorManager()->getSectorForCoordinates(minusLoc.x, minusLoc.y);
            bool minusOkay = newSector->checkMovement(curRound, stepReach, minusLoc);
            
            if (plusOkay) {
                if (minusOkay) {
                    // okay
                } else {
                    direction = plusDirection;
                    outLoc = plusLoc;
                }
            } else {
                if (minusOkay) {
                    direction = minusDirection;
                    outLoc = minusLoc;
                } else {
                    // blocked
                    sectorOkay = false;
                }
            }
            
        } else {
            // maybe we can move in an adjacent direction (+1)
            unsigned int newDirection = (direction & ~0x7) | ((direction + 1) & 0x7);
            diff = getDirectionOffset(newDirection);
            outLoc = curRound + diff;
            newSector = world::Manager::getSectorManager()->getSectorForCoordinates(outLoc.x, outLoc.y);
            sectorOkay = newSector->checkMovement(curRound, stepReach, outLoc);
            
            if (sectorOkay) {
                direction = newDirection;
            } else {
                // check other direction (-1)
                unsigned int newDirection = (direction & ~0x7) | ((direction - 1) & 0x7);
                diff = getDirectionOffset(newDirection);
                outLoc = curRound + diff;
                newSector = world::Manager::getSectorManager()->getSectorForCoordinates(outLoc.x, outLoc.y);
                sectorOkay = newSector->checkMovement(curRound, stepReach, outLoc);
                
                if (sectorOkay) {
                    direction = newDirection;
                }
            } 
        }
    }
    
    return sectorOkay;
}

CL_Vec3f PlayerWalkManager::getDirectionOffset(unsigned int direction) const {
    switch (direction & 0x7) {
        case Direction::N: return CL_Vec3f(0, -1, 0);
        case Direction::NE: return CL_Vec3f(1, -1, 0);
        case Direction::E:  return CL_Vec3f(1, 0, 0);
        case Direction::SE: return CL_Vec3f(1, 1, 0);
        case Direction::S: return CL_Vec3f(0, 1, 0);
        case Direction::SW: return CL_Vec3f(-1, 1, 0);
        case Direction::W: return CL_Vec3f(-1, 0, 0);
        case Direction::NW: return CL_Vec3f(-1, -1, 0);
    }
    
    return CL_Vec3f(0, 0, 0);
}

}
}

