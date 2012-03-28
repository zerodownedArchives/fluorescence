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


#include "playsound.hpp"

#include <ui/audiomanager.hpp>
#include <ui/manager.hpp>

namespace fluo {
namespace net {
namespace packets {

PlaySound::PlaySound() : Packet(0x54, 12) {
}

bool PlaySound::read(const int8_t* buf, unsigned int len, unsigned int& index) {
    bool ret = true;
    
    ret &= PacketReader::read(buf, len, index, playMode_);
    ret &= PacketReader::read(buf, len, index, soundId_);
    
    uint16_t unknown;
    ret &= PacketReader::read(buf, len, index, unknown);
    
    ret &= PacketReader::read(buf, len, index, locX_);
    ret &= PacketReader::read(buf, len, index, locY_);
    ret &= PacketReader::read(buf, len, index, locZ_);

    return ret;
}

void PlaySound::onReceive() {
    ui::Manager::getAudioManager()->playSound(soundId_);
}

}
}
}
