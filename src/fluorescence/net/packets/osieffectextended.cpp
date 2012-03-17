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



#include "osieffectextended.hpp"

namespace fluo {
namespace net {
namespace packets {

OsiEffectExtended::OsiEffectExtended() : OsiEffect(0xC7, 49) {
}

bool OsiEffectExtended::read(const int8_t* buf, unsigned int len, unsigned int& index) {
    bool ret = OsiEffect::read(buf, len, index);;
    
    ret &= PacketReader::read(buf, len, index, effectArtId_);
    ret &= PacketReader::read(buf, len, index, explodeArtId_);
    ret &= PacketReader::read(buf, len, index, explodeSound_);
    ret &= PacketReader::read(buf, len, index, unknownSerial_);
    ret &= PacketReader::read(buf, len, index, layer_);
    
    // jump 2 unknown bytes
    index += 2;

    return ret;
}

}
}
}
