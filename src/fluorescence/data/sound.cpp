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
