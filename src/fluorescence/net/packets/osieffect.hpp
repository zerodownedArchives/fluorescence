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


#ifndef FLUO_NET_PACKETS_OSIEFFECT_HPP
#define FLUO_NET_PACKETS_OSIEFFECT_HPP

#include <net/packet.hpp>
#include <typedefs.hpp>

namespace fluo {
namespace net {

namespace packets {

class OsiEffect : public Packet {
public:
    OsiEffect(uint8_t packetId = 0xC0, uint16_t packetLen = 36);

    virtual bool read(const int8_t* buf, unsigned int len, unsigned int& index);

    virtual void onReceive();

private:
    uint8_t effectType_;
    Serial sourceSerial_;
    Serial targetSerial_;
    uint16_t artId_;
    
    uint16_t sourceX_;
    uint16_t sourceY_;
    int8_t sourceZ_;
    uint16_t targetX_;
    uint16_t targetY_;
    int8_t targetZ_;
    
    uint8_t speed_;
    uint8_t duration_;
    uint8_t fixedDirection_;
    uint8_t explode_;
    uint32_t hue_;
    uint32_t renderMode_;
};

}
}
}

#endif
