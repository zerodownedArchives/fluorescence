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


#ifndef FLUO_NET_WALKPACKETMANAGER_HPP
#define FLUO_NET_WALKPACKETMANAGER_HPP

#include <boost/shared_ptr.hpp>
#include <stdint.h>

namespace fluo {

namespace world {
class Mobile;
}

namespace net {

namespace packets {
class MovementDeny;
}

class WalkPacketManager {
public:
    WalkPacketManager();

    void initFastWalkStack(const uint32_t* values);
    void updateFastWalkStack(uint32_t value);

    void onMovementAccept(uint8_t sequence);
    void onMovementDeny(const net::packets::MovementDeny* pkt);
    bool sendMovementRequest(uint8_t direction);

private:
    uint32_t fastWalkStack_[6];
    unsigned int fastWalkStackIdx_;
    uint8_t curSequence_;

    unsigned int unacceptedSequenceCount_;
    uint8_t unacceptedSequences_[10];
    static const unsigned int MAX_UNACCEPTED_SEQUENCE_COUNT = 5;
};

}
}

#endif
