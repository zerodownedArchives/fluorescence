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


#ifndef FLUO_NET_PACKETS_CHARACTERLIST_HPP
#define FLUO_NET_PACKETS_CHARACTERLIST_HPP

#include <net/packet.hpp>
#include <misc/string.hpp>

#include <vector>
#include <ClanLib/Core/Math/vec4.h>

namespace fluo {
namespace net {

namespace packets {

class CharacterList : public Packet {
public:
    CharacterList();

    virtual bool read(const int8_t* buf, unsigned int len, unsigned int& index);

    virtual void onReceive();


    uint8_t charCount_;
    UnicodeString charNames_[7];
    UnicodeString charPasswords_[7];

    uint8_t cityCount_;
    std::vector<uint8_t> cityIndices_;
    std::vector<UnicodeString> cityNames_;
    std::vector<UnicodeString> tavernNames_;
    std::vector<CL_Vec4f> cityCoordinates_;
    std::vector<unsigned int> cityCliloc_;

    uint32_t flags_;
};

}
}
}

#endif
