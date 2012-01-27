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



#include "walkmanager.hpp"

#include <string.h>

#include <world/manager.hpp>
#include <world/smoothmovementmanager.hpp>
#include <world/mobile.hpp>

#include <misc/log.hpp>

#include "manager.hpp"
#include "packets/movementdeny.hpp"
#include "packets/movementrequest.hpp"

namespace fluo {
namespace net {

WalkManager::WalkManager() : fastWalkStackIdx_(0), curSequence_(0), unacceptedSequenceCount_(0) {
}

void WalkManager::initFastWalkStack(const uint32_t* stack) {
    memcpy(fastWalkStack_, stack, sizeof(uint32_t) * 6);
}

void WalkManager::updateFastWalkStack(uint32_t value) {
    for (unsigned int i = 5; i > 0; ++i) {
        fastWalkStack_[i] = fastWalkStack_[i-1];
    }
    fastWalkStack_[0] = value;
}

void WalkManager::onMovementAccept(uint8_t sequence) {
    LOG_DEBUG << "movement accept: " << (unsigned int)sequence << std::endl;
    if (!player_) {
        player_ = world::Manager::getSingleton()->getPlayer();
    }

    unsigned int i = 0;
    for (; i < unacceptedSequenceCount_; ++i) {
        if (unacceptedSequences_[i] == sequence) {
            break;
        }
    }

    if (i == unacceptedSequenceCount_) {
        // something went wrong. send resync request?
    } else {
        // copy not accepted sequence numbers
        for (unsigned int j = 0, k = i; k < unacceptedSequenceCount_; ++k, ++j) {
            unacceptedSequences_[j] = unacceptedSequences_[k];
        }

        unacceptedSequenceCount_ -= (i + 1);
    }
}

void WalkManager::onMovementDeny(const net::packets::MovementDeny* pkt) {
    LOG_DEBUG << "movement deny" << std::endl;
    if (!player_) {
        player_ = world::Manager::getSingleton()->getPlayer();
    }

    player_->setLocation(pkt->locX_, pkt->locY_, pkt->locZ_);
    player_->setDirection(pkt->direction_);

    curSequence_ = 0;
    fastWalkStackIdx_ = 0;

    world::Manager::getSmoothMovementManager()->clear(player_->getSerial());

    unacceptedSequenceCount_ = 0;
}

void WalkManager::onMovementRequest(uint8_t direction) {
    if (!player_) {
        player_ = world::Manager::getSingleton()->getPlayer();
    }

    // to many not acknowledged yet
    if (unacceptedSequenceCount_ >= MAX_UNACCEPTED_SEQUENCE_COUNT) {
        return;
    }

    uint8_t newSequence = curSequence_;
    ++curSequence_;

    uint32_t newFastWalkKey = fastWalkStack_[fastWalkStackIdx_];
    fastWalkStackIdx_ = (fastWalkStackIdx_ + 1) % 6;

    // create packet
    packets::MovementRequest pkt(direction, newSequence, newFastWalkKey);
    net::Manager::getSingleton()->send(pkt);

    // add unaccepted sequence number
    unacceptedSequences_[unacceptedSequenceCount_] = newSequence;
    ++unacceptedSequenceCount_;

    // start to move player there
    if (player_->getDirection() != direction) {
        // TODO: this should be delayed
        player_->setDirection(direction);
    } else {
        // TODO: play animation
        world::SmoothMovement mov(player_, direction, 300);
        world::Manager::getSmoothMovementManager()->add(player_->getSerial(), mov);
        player_->playAnim(player_->getWalkAnim());
    }
}

}
}
