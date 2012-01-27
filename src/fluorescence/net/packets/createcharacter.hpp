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


#ifndef FLUO_NET_PACKETS__HPP
#define FLUO_NET_PACKETS__HPP

#include <net/packet.hpp>

namespace fluo {
namespace net {

namespace packets {

class CreateCharacter : public Packet {
public:
    CreateCharacter();

    virtual bool write(int8_t* buf, unsigned int len, unsigned int& index) const;
    

    UnicodeString name_;
    UnicodeString password_;
    bool female_;
    
    uint8_t strength_;
    uint8_t dexterity_;
    uint8_t intelligence_;
    
    uint8_t skill1Id_;
    uint8_t skill1Value_;
    uint8_t skill2Id_;
    uint8_t skill2Value_;
    uint8_t skill3Id_;
    uint8_t skill3Value_;
    
    uint16_t hue_;
    uint16_t hairStyle_;
    uint16_t hairHue_;
    uint16_t beardStyle_;
    uint16_t beardHue_;
    
    uint16_t startCity_;
    
    uint16_t slot_;
    
    uint32_t encryptionKey_;
    
    uint16_t shirtHue_;
    uint16_t pantsHue_;
};

}
}
}

#endif
