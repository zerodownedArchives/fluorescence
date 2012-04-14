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



#include "bitmask.hpp"

#include <string.h>

namespace fluo {
namespace ui {

BitMask::BitMask() : width_(0), height_(0), bitStoreSize_(0), bitStore_(NULL) {

}

BitMask::~BitMask() {
    if (bitStore_) {
        free(bitStore_);
        bitStore_ = NULL;
    }
}

bool BitMask::hasPixel(unsigned int pixelX, unsigned int pixelY) {
    if (pixelX >= width_ || pixelY >= height_) {
        return false;
    }

    unsigned int bitIndex = pixelY * width_ + pixelX;
    unsigned int byteIndex = bitIndex / 8;
    unsigned int bitOffset = bitIndex % 8;

    return bitStore_[byteIndex] & (1 << bitOffset);
}

void BitMask::setPixel(unsigned int pixelX, unsigned int pixelY) {
    unsigned int bitIndex = pixelY * width_ + pixelX;
    unsigned int byteIndex = bitIndex / 8;
    unsigned int bitOffset = bitIndex % 8;

    bitStore_[byteIndex] |= (1 << bitOffset);
}

void BitMask::init(const CL_PixelBuffer& pixBuf) {
    width_ = pixBuf.get_width();
    height_ = pixBuf.get_height();
    bitStoreSize_ = ((width_ * height_) / 8) + 1;
    bitStore_ = reinterpret_cast<uint8_t*>(malloc(bitStoreSize_));
    memset(bitStore_, 0, bitStoreSize_);

    const uint32_t* bufPtr = reinterpret_cast<const uint32_t*>(pixBuf.get_data());

    for (unsigned int y = 0; y < height_; ++y) {
        for (unsigned int x = 0; x < width_; ++x) {
            if (*bufPtr & 0x000000FFu) { // alpha != 0
                setPixel(x, y);
            }
            ++bufPtr;
        }
    }
}

}
}
