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


#ifndef FLUO_NET_PACKETREADER_HPP
#define FLUO_NET_PACKETREADER_HPP

#include <misc/string.hpp>

namespace fluo {
namespace net {

class PacketReader {
public:
    static bool readUnicodeNull(const int8_t* buf, unsigned int len, unsigned int& index, UnicodeString& value);
    static bool readUnicodeFixed(const int8_t* buf, unsigned int len, unsigned int& index, UnicodeString& value, unsigned int numChars);

    static bool readUtf8Null(const int8_t* buf, unsigned int len, unsigned int& index, UnicodeString& value);
    static bool readUtf8Fixed(const int8_t* buf, unsigned int len, unsigned int& index, UnicodeString& value, unsigned int fixed);

    static bool readUnicodeNullLE(const int8_t* buf, unsigned int len, unsigned int& index, UnicodeString& value);

    static bool read(const int8_t* buf, unsigned int len, unsigned int& index, uint8_t& value);
    static bool read(const int8_t* buf, unsigned int len, unsigned int& index, uint16_t& value);
    static bool read(const int8_t* buf, unsigned int len, unsigned int& index, uint32_t& value);
    static bool read(const int8_t* buf, unsigned int len, unsigned int& index, int8_t& value);
    static bool read(const int8_t* buf, unsigned int len, unsigned int& index, int16_t& value);
    static bool read(const int8_t* buf, unsigned int len, unsigned int& index, int32_t& value);

private:
    static int8_t* fixedStringBuf_;
};

}
}


#endif
