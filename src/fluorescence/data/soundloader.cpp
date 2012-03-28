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


#include "soundloader.hpp"

#include <ui/manager.hpp>
#include <ui/audiomanager.hpp>

namespace fluo {
namespace data {
    
SoundLoader::SoundLoader(const boost::filesystem::path& idxPath, const boost::filesystem::path& mulPath) :
        loader_(idxPath, mulPath, boost::bind(&SoundLoader::readCallback, this, _1, _2, _3, _4, _5, _6)) {
}

void SoundLoader::readCallback(unsigned int index, int8_t* buf, unsigned int len, boost::shared_ptr<OnDemandReadable<void> >, unsigned int extra, unsigned int userData) {
    char waveBuf[len + 4]; // 40 bytes of data ignored (20 bytes filename, 20 unknown bytes), but need 44 byte header
    
    unsigned int dataLen = len - 40;
    // set up file header
    WaveHeader header(dataLen);
    
    memcpy(waveBuf, &header, sizeof(WaveHeader));
    memcpy(waveBuf + sizeof(WaveHeader), buf, dataLen);
    
    ui::Manager::getAudioManager()->playSoundFromMul(waveBuf, len + 4);
}

void SoundLoader::loadAndPlay(unsigned int soundId) {
    loader_.get(soundId, 0);
}

}
}

