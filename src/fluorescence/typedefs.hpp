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

struct AnimRepeatMode {
enum {
    LOOP = 0, // loop animation forever
    LAST = 1, // play animation once, then stick with the last frame
    DEFAULT = 2, // play animation once, then switch to to default frame
};
};

struct AnimType {
enum {
    HIGH_DETAIL = 0,
    LOW_DETAIL = 1,
    PEOPLE = 2,
};
};

struct IsoIndex {
public:
    IsoIndex() : value_(0), x_(0), y_(0) { }
    IsoIndex(unsigned int x, unsigned int y) : x_(x & 0xFFFf), y_(y & 0xFFFF) {
        value_ = x_ + y_;
        value_ <<= 32;
        value_ |= (x_ << 16) | y_;
    }

    bool operator>(const IsoIndex& other) const { return value_ > other.value_; }
    bool operator<(const IsoIndex& other) const { return value_ < other.value_; }
    bool operator==(const IsoIndex& other) const { return value_ == other.value_; }
    bool operator!=(const IsoIndex& other) const { return value_ != other.value_; }

    uint64_t value_;
    uint16_t x_;
    uint16_t y_;
};

struct RenderDepth {
    RenderDepth() : value_(0) { }
    RenderDepth(uint64_t value) : value_(value) { }
    
    bool operator>(const RenderDepth& other) const { return value_ > other.value_; }
    bool operator<(const RenderDepth& other) const { return value_ < other.value_; }
    bool operator==(const RenderDepth& other) const { return value_ == other.value_; }
    bool operator!=(const RenderDepth& other) const { return value_ != other.value_; }
    
    uint64_t value_;
};

}


#endif
