
#include "packetreader.hpp"

#include <string.h>
#include <stdlib.h>

namespace uome {
namespace net {

int8_t* PacketReader::fixedStringBuf_ = reinterpret_cast<int8_t*>(malloc(0x4000));

bool PacketReader::readUtf8Null(const int8_t* buf, unsigned int len, unsigned int& index, UnicodeString& value) {
    // find 0 byte, required to set index
    unsigned int origIndex = index;

    while (buf[index++] != 0);

    value = StringConverter::fromUtf8(&buf[origIndex], -1);

    static UnicodeString errorIndicator("##UOMEERROR");

    if (value == errorIndicator) {
        return false;
    } else {
        return true;
    }
}

bool PacketReader::readUtf8Fixed(const int8_t* buf, unsigned int len, unsigned int& index, UnicodeString& value, unsigned int fixed) {
    if (index + fixed < len) {
        value = StringConverter::fromUtf8(&buf[index], fixed);

        static UnicodeString errorIndicator("##UOMEERROR");

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

    while (reinterpret_cast<const uint16_t*>(buf)[index++] != 0);
    ++index; // two 0 bytes this time

    value = StringConverter::fromUnicode(&buf[origIndex], -1);

    static UnicodeString errorIndicator("##UOMEERROR");

    if (value == errorIndicator) {
        return false;
    } else {
        return true;
    }
}

bool PacketReader::readUnicodeFixed(const int8_t* buf, unsigned int len, unsigned int& index, UnicodeString& value, unsigned int fixed) {
    if (index + fixed < len) {
        value = StringConverter::fromUnicode(&buf[index], fixed);

        static UnicodeString errorIndicator("##UOMEERROR");

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

}
}
