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


#ifndef FLUO_TYPEDEFS_HPP
#define FLUO_TYPEDEFS_HPP

#include <stdint.h>

namespace fluo {

typedef uint32_t Serial;

struct Direction {
public:
enum {
    N = 0x00,
    NE = 0x01,
    E = 0x02,
    SE = 0x03,
    S = 0x04,
    SW = 0x05,
    W = 0x06,
    NW = 0x07,

    RUNNING = 0x80,
};
};


struct TextType {
public:
enum {
    NORMAL = 0x00,
    BROADCAST = 0x01,
    EMOTE = 0x02,
    SYSTEM = 0x06,
    MESSAGE = 0x07,
    WHISPER = 0x08,
    YELL = 0x09,
};
};

struct Layer {
public:
enum {
    ONEHANDED = 0x01,
    TWOHANDED = 0x02,
    MOUNT = 0x19,
};
};

}


#endif
