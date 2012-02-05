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



#include "packetreader.hpp"

#include <string.h>
#include <stdlib.h>

#ifdef WIN32
#include <WinSock2.h>
#else
#include <netinet/in.h>
#endif

namespace fluo {
namespace net {

int8_t* PacketReader::fixedStringBuf_ = reinterpret_cast<int8_t*>(malloc(0x4000));

bool PacketReader::readUtf8Null(const int8_t* buf, unsigned int len, unsigned int& index, UnicodeString& value) {
    // find 0 byte, required to set index
    unsigned int origIndex = index;

    while (buf[index++] != 0);

    value = StringConverter::fromUtf8(&buf[origIndex], (index - origIndex));

    static UnicodeString errorIndicator("##FLUOERROR");

    if (value == errorIndicator) {
        return false;
    } else {
        return true;
    }
}

bool PacketReader::readUtf8Fixed(const int8_t* buf, unsigned int len, unsigned int& index, UnicodeString& value, unsigned int fixed) {
    if (index + fixed <= len) {
        value = StringConverter::fromUtf8(&buf[index], fixed);

        static UnicodeString errorIndicator("##FLUOERROR");

        if (value == errorIndicator) {
            return false;
        } else {
            index += fixed;
            return true;
        }
    } else {
        return false;
    }
}

bool PacketReader::readUnicodeNull(const int8_t* buf, unsigned int len, unsigned int& index, UnicodeString& value) {
    // find 00 bytes, required to set index
    unsigned int origIndex = index;

    while (*(reinterpret_cast<const uint16_t*>(&buf[index])) != 0) {
        index += 2;
    }
    index += 2;

    value = StringConverter::fromUnicode(&buf[origIndex], (index - origIndex));

    static UnicodeString errorIndicator("##FLUOERROR");

    if (value == errorIndicator) {
        return false;
    } else {
        return true;
    }
}

bool PacketReader::readUnicodeNullLE(const int8_t* buf, unsigned int len, unsigned int& index, UnicodeString& value) {
    // find 00 bytes, required to set index
    unsigned int origIndex = index;

    while (*(reinterpret_cast<const uint16_t*>(&buf[index])) != 0) {
        index += 2;
    }
    index += 2;

    value = StringConverter::fromUnicodeLE(&buf[origIndex], (index - origIndex));

    static UnicodeString errorIndicator("##FLUOERROR");

    if (value == errorIndicator) {
        return false;
    } else {
        return true;
    }
}

bool PacketReader::readUnicodeFixed(const int8_t* buf, unsigned int len, unsigned int& index, UnicodeString& value, unsigned int numChars) {
    unsigned int numBytes = numChars * 2; // 2 bytes per char
    if (index + numBytes <= len) {
        value = StringConverter::fromUnicode(&buf[index], numBytes);

        static UnicodeString errorIndicator("##FLUOERROR");

        if (value == errorIndicator) {
            return false;
        } else {
            index += numBytes;
            return true;
        }
    } else {
        return false;
    }
}

bool PacketReader::read(const int8_t* buf, unsigned int len, unsigned int& index, uint8_t& value) {
    if (index + sizeof(uint8_t) > len) {
        return false;
    }

    value = *reinterpret_cast<const uint8_t*>(&buf[index]);

    index += sizeof(uint8_t);

    return true;
}

bool PacketReader::read(const int8_t* buf, unsigned int len, unsigned int& index, uint16_t& value) {
    if (index + sizeof(uint16_t) > len) {
        return false;
    }

    value = *reinterpret_cast<const uint16_t*>(&buf[index]);
    value = ntohs(value);

    index += sizeof(uint16_t);

    return true;
}

bool PacketReader::read(const int8_t* buf, unsigned int len, unsigned int& index, uint32_t& value) {
    if (index + sizeof(uint32_t) > len) {
        return false;
    }

    value = *reinterpret_cast<const uint32_t*>(&buf[index]);
    value = ntohl(value);

    index += sizeof(uint32_t);

    return true;
}


bool PacketReader::read(const int8_t* buf, unsigned int len, unsigned int& index, int8_t& value) {
    if (index + sizeof(int8_t) > len) {
        return false;
    }

    value = buf[index];

    index += sizeof(int8_t);

    return true;
}


bool PacketReader::read(const int8_t* buf, unsigned int len, unsigned int& index, int16_t& value) {
    if (index + sizeof(int16_t) > len) {
        return false;
    }

    value = *reinterpret_cast<const int16_t*>(&buf[index]);
    value = ntohs(value);

    index += sizeof(int16_t);

    return true;
}


bool PacketReader::read(const int8_t* buf, unsigned int len, unsigned int& index, int32_t& value) {
    if (index + sizeof(int32_t) > len) {
        return false;
    }

    value = *reinterpret_cast<const int32_t*>(&buf[index]);
    value = ntohl(value);

    index += sizeof(int32_t);

    return true;
}



}
}
