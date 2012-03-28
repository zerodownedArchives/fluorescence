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


#ifndef FLUO_DATA_SOUNDLOADER_HPP
#define FLUO_DATA_SOUNDLOADER_HPP

#include "indexedondemandfileloader.hpp"

#include "ondemandreadable.hpp"

namespace fluo {
namespace data {
    
struct WaveHeader {
    char txtRiff_[4];
    uint32_t totalLength_;
    char txtWave_[4];
    
    char txtFmt_[4];
    uint32_t chunkSize_;
    uint16_t formatTag_;
    uint16_t numChannels_;
    uint32_t sampleRate_;
    uint32_t byteRate_;
    uint16_t blockAlign_;
    uint16_t bitsPerSample_;
    
    char txtData_[4];
    uint32_t dataLength_;
    
    WaveHeader(unsigned int dataLen) : 
            txtRiff_({'R', 'I', 'F', 'F'}), totalLength_(dataLen + 36), txtWave_({'W', 'A', 'V', 'E'}),
            txtFmt_({'f', 'm', 't', ' '}), chunkSize_(16), formatTag_(1), numChannels_(1), sampleRate_(22050), byteRate_(88200), blockAlign_(4), bitsPerSample_(16),
            txtData_({'d', 'a', 't', 'a'}), dataLength_(dataLen) {
    }
};

// The SoundLoader is a bit different than the other loaders, because it sets some action as soon as the data chunk is read
// (namely playing the sound using the audio manager)
// Thus, the sound data is not cached
class SoundLoader {
public:
    SoundLoader(const boost::filesystem::path& idxPath, const boost::filesystem::path& mulPath);

    void loadAndPlay(unsigned int soundId);

    void readCallback(unsigned int index, int8_t* buf, unsigned int len, boost::shared_ptr<OnDemandReadable<void> >, unsigned int extra, unsigned int userData);
    
private:
    // Data is not cached, so the second parameter is just a dummy
    IndexedOnDemandFileLoader<unsigned int, OnDemandReadable<void> > loader_;

};

}
}

#endif

