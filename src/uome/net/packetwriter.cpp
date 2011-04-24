
#include "packetwriter.hpp"

#include <string.h>
#include <stdlib.h>

namespace uome {
namespace net {

int8_t* PacketWriter::fixedStringBuf_ = reinterpret_cast<int8_t*>(malloc(0x4000));

bool PacketWriter::writeUtf8Null(int8_t* buf, unsigned int len, unsigned int& index, const UnicodeString& value) {
    int bytecount = StringConverter::toUtf8(value, &buf[index], len - index, true);

    if (bytecount == -1) {
        return false;
    } else {
        index += bytecount;
        return true;
    }
}

bool PacketWriter::writeUtf8Fixed(int8_t* buf, unsigned int len, unsigned int& index, const UnicodeString& value, unsigned int fixed) {
    if (index + fixed < len) {
        int bytes = StringConverter::toUtf8(value, fixedStringBuf_, 0x4000, false);

        if (bytes == -1) {
            return false;
        }

        if ((unsigned int)bytes < fixed) {
            memcpy(&buf[index], fixedStringBuf_, bytes);
            // set other bytes to zero
            memset(&buf[index + bytes], 0, fixed - bytes);
        } else {
            // copy all bytes that fit in the fixed size
            memcpy(&buf[index], fixedStringBuf_, fixed);
        }

        index += fixed;
        return true;
    } else {
        return false;
    }
}

bool PacketWriter::writeUnicodeNull(int8_t* buf, unsigned int len, unsigned int& index, const UnicodeString& value) {
    int bytecount = StringConverter::toUnicode(value, &buf[index], len - index, true);

    if (bytecount == -1) {
        return false;
    } else {
        index += bytecount;
        return true;
    }
}

bool PacketWriter::writeUnicodeFixed(int8_t* buf, unsigned int len, unsigned int& index, const UnicodeString& value, unsigned int fixed) {
    if (index + fixed < len) {
        int bytes = StringConverter::toUnicode(value, fixedStringBuf_, 0x4000, false);

        if (bytes == -1) {
            return false;
        }

        if ((unsigned int)bytes < fixed) {
            memcpy(&buf[index], fixedStringBuf_, bytes);
            // set other bytes to zero
            memset(&buf[index + bytes], 0, fixed - bytes);
        } else {
            // copy all bytes that fit in the fixed size
            memcpy(&buf[index], fixedStringBuf_, fixed);
        }

        index += fixed;
        return true;
    } else {
        return false;
    }
}

}
}
