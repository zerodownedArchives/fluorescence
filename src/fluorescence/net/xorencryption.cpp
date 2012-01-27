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



#include "xorencryption.hpp"

#include <string.h>

namespace fluo {
namespace net {

XorEncryption::XorEncryption(uint32_t seed, uint32_t key1, uint32_t key2) {
    seed_ = seed;
    baseKey1_ = key1;
    baseKey2_ = key2;

    uint32_t swappedSeed = 0;

    swappedSeed = (seed_ & 0xFF000000) >> 24;
    swappedSeed |= (seed_ & 0x00FF0000) >> 8;
    swappedSeed |= (seed_ & 0x0000FF00) << 8;
    swappedSeed |= (seed_ & 0x000000FF) << 24;

    key1_ = ((~swappedSeed ^ 0x00001357 ) << 16) | ((swappedSeed ^ 0xFFFFAAAA)
            & 0x0000FFFF);
    key2_ = ((swappedSeed ^ 0x43210000) >> 16) | ((~swappedSeed ^ 0xABCDFFFF)
            & 0xFFFF0000);
}

void XorEncryption::encrypt(int8_t* dst, const int8_t* src, unsigned int length) {
    uint32_t temp1 = 0;
    uint32_t temp2 = 0;

    for (unsigned int i = 0; i < length; i++) {
        dst[i] = src[i] ^ (key1_ & 0x000000FF);
        temp1 = key1_;
        temp2 = key2_;

        key1_ = ((temp1 >> 1) | (temp2 << 31)) ^ baseKey2_;
        temp2 = ((temp2 >> 1) | (temp1 << 31)) ^ baseKey1_;
        key2_ = ((temp2 >> 1) | (temp1 << 31)) ^ baseKey1_;
    }
}

void XorEncryption::decrypt(int8_t* dst, const int8_t* src, unsigned int length) {
    memcpy(dst, src, length);
}

}
}
