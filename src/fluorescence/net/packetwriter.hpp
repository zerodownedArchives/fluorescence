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


#ifndef FLUO_NET_PACKETWRITER_HPP
#define FLUO_NET_PACKETWRITER_HPP

#include <misc/string.hpp>

namespace fluo {
namespace net {

class PacketWriter {
public:
    static bool writeUnicodeNull(int8_t* buf, unsigned int len, unsigned int& index, const UnicodeString& value);
    static bool writeUnicodeFixed(int8_t* buf, unsigned int len, unsigned int& index, const UnicodeString& value, unsigned int fixed);

    static bool writeUtf8Null(int8_t* buf, unsigned int len, unsigned int& index, const UnicodeString& value);
    static bool writeUtf8Fixed(int8_t* buf, unsigned int len, unsigned int& index, const UnicodeString& value, unsigned int fixed);

    static bool write(int8_t* buf, unsigned int len, unsigned int& index, const uint8_t& value);
    static bool write(int8_t* buf, unsigned int len, unsigned int& index, const uint16_t& value);
    static bool write(int8_t* buf, unsigned int len, unsigned int& index, const uint32_t& value);
    static bool write(int8_t* buf, unsigned int len, unsigned int& index, const int8_t& value);
    static bool write(int8_t* buf, unsigned int len, unsigned int& index, const int16_t& value);
    static bool write(int8_t* buf, unsigned int len, unsigned int& index, const int32_t& value);

private:
    static int8_t* fixedStringBuf_;
};

}
}


#endif
