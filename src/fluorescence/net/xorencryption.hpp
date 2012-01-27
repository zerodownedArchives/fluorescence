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


#ifndef FLUO_NET_XORENCRYTPTION_HPP
#define FLUO_NET_XORENCRYTPTION_HPP

#include "encryption.hpp"

namespace fluo {
namespace net {

class XorEncryption : public Encryption {
public:
    XorEncryption(uint32_t seed, uint32_t key1, uint32_t key2);

    virtual void encrypt(int8_t* dst, const int8_t* src, unsigned int length);
    virtual void decrypt(int8_t* dst, const int8_t* src, unsigned int length);

private:
    uint32_t seed_;

    uint32_t baseKey1_;
    uint32_t baseKey2_;

    uint32_t key1_;
    uint32_t key2_;
};

}
}

#endif
