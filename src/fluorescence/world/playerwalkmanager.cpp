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

#include <net/manager.hpp>
#include <net/walkpacketmanager.hpp>

#include <misc/log.hpp>

namespace fluo {
namespace world {

PlayerWalkManager::PlayerWalkManager() : isWalking_(false), lastIsWalking_(false) {
}
    
void PlayerWalkManager::setWalkDirection(uint8_t direction) {
    isWalking_ = true;
    requestedDirection_ = direction;
}

void PlayerWalkManager::stopAtNextTile() {
    isWalking_ = false;
}

void PlayerWalkManager::stopImmediately() {
    isWalking_ = false;
    world::Manager::getSmoothMovementManager()->clear(world::Manager::getSingleton()->getPlayer()->getSerial());
}

void PlayerWalkManager::update(unsigned int elapsedMillis) {
    if (!isWalking_) {
        lastIsWalking_ = false;
        // anim is ended automatically by smooth movement object
        return;
    }
    
    boost::shared_ptr<world::Mobile> player = world::Manager::getSingleton()->getPlayer();
    
    if (!lastIsWalking_) {
        // starting to walk now => start anim
        player->animate(player->getWalkAnim(), 0, AnimRepeatMode::LOOP);
        millisToNextMove_ = 0;
    } else {
        millisToNextMove_ -= elapsedMillis;
    }
    
    // don't wait until the smooth movement is completely over to avoid jerking motions
    if (millisToNextMove_ <= 0) {
        if (player->getDirection() != requestedDirection_) {
            if (!lastIsWalking_) {
                // just turn and wait for a bit
                millisToNextMove_ = 150;
            }
            
            player->setDirection(requestedDirection_);
            net::Manager::getWalkPacketManager()->sendMovementRequest(requestedDirection_);
        }
        
        if (lastIsWalking_) {
            // TODO: check running, mounted. different speed
            unsigned int moveDuration = 100;
            
            millisToNextMove_ = moveDuration;;
            
            world::SmoothMovement mov(player, requestedDirection_, moveDuration);
            mov.setFinishedCallback(boost::bind(&PlayerWalkManager::onSmoothMovementFinish, this));
            world::Manager::getSmoothMovementManager()->add(player->getSerial(), mov);
            net::Manager::getWalkPacketManager()->sendMovementRequest(requestedDirection_);
        }
        
        lastIsWalking_ = true;
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

}
}

