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

#include "sound.hpp"

namespace fluo {
namespace data {

WaveHeader::WaveHeader(unsigned int dataLen) : 
            totalLength_(dataLen + 36), 
            chunkSize_(16), formatTag_(1), numChannels_(1), sampleRate_(22050), byteRate_(88200), blockAlign_(4), bitsPerSample_(16),
            dataLength_(dataLen) {
	txtRiff_[0] = 'R';  txtRiff_[1] = 'I'; txtRiff_[2] = 'F'; txtRiff_[3] = 'F';
	txtWave_[0] = 'W'; txtWave_[1] = 'A'; txtWave_[2] = 'V'; txtWave_[3] = 'E';
	txtFmt_[0] = 'f'; txtFmt_[1] = 'm'; txtFmt_[2] = 't'; txtFmt_[3] = ' ';
	txtData_[0] = 'd'; txtData_[1] = 'a'; txtData_[2] = 't'; txtData_[3] = 'a';
}

Sound::Sound() : dataLength_(0), rawData_(nullptr) {
}

Sound::~Sound() {
    if (rawData_) {
        free(rawData_);
    }
}

void Sound::setName(const int8_t* nameRaw) {
    name_ = StringConverter::fromUtf8(nameRaw, 20);
}

void Sound::setData(const int8_t* buf, unsigned int len) {
    dataLength_ = len + 44;
    rawData_ = (char*)malloc(dataLength_);
    
    WaveHeader header(len);
    memcpy(rawData_, &header, sizeof(WaveHeader));
    memcpy(rawData_ + sizeof(WaveHeader), buf, len);
}

unsigned int Sound::getDataLength() const {
    return dataLength_;
}

const char* Sound::getData() const {
    return rawData_;
}

const UnicodeString& Sound::getName() const {
    return name_;
}

}
}
