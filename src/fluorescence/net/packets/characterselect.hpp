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


#ifndef FLUO_NET_PACKETS_CHARACTERSELECT_HPP
#define FLUO_NET_PACKETS_CHARACTERSELECT_HPP

#include <net/packet.hpp>

namespace fluo {
namespace net {

namespace packets {

class CharacterSelect : public Packet {
public:
    CharacterSelect(const UnicodeString& name, const UnicodeString& password, uint32_t index, uint32_t seed);

    virtual bool write(int8_t* buf, unsigned int len, unsigned int& index) const;

private:
    UnicodeString charName_;
    UnicodeString charPassword_;
    uint32_t charIndex_;
    uint32_t seed_;
};

}
}
}

#endif
