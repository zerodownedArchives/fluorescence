
#include "packetwriter.hpp"

#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>

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

bool PacketWriter::writeUtf8Null(int8_t* buf, unsigned int len, unsigned int& index, const std::string& value) {
    if (index + value.length() < len) {
        memcpy(&buf[index], value.c_str(), value.length());
        index += value.length();
        return true;
    } else {
        return false;
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

bool PacketWriter::writeUtf8Fixed(int8_t* buf, unsigned int len, unsigned int& index, const std::string& value, unsigned int fixed) {
    if (index + fixed < len) {
        int bytes = value.length();

        if ((unsigned int)bytes < fixed) {
            memcpy(&buf[index], value.c_str(), bytes);
            // set other bytes to zero
            memset(&buf[index + bytes], 0, fixed - bytes);
        } else {
            // copy all bytes that fit in the fixed size
            memcpy(&buf[index], value.c_str(), fixed);
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

bool PacketWriter::write(int8_t* buf, unsigned int len, unsigned int& index, const uint8_t& value) {
    if (index + sizeof(uint8_t) > len) {
        return false;
    }

    *reinterpret_cast<uint8_t*>(&buf[index]) = value;

    index += sizeof(uint8_t);

    return true;
}

bool PacketWriter::write(int8_t* buf, unsigned int len, unsigned int& index, const uint16_t& value) {
    if (index + sizeof(uint16_t) > len) {
        return false;
    }

    uint16_t byteOrder = htons(value);
    *reinterpret_cast<uint16_t*>(&buf[index]) = byteOrder;

    index += sizeof(uint16_t);

    return true;
}

bool PacketWriter::write(int8_t* buf, unsigned int len, unsigned int& index, const uint32_t& value) {
    if (index + sizeof(uint32_t) > len) {
        return false;
    }

    uint32_t byteOrder = htonl(value);
    *reinterpret_cast<uint32_t*>(&buf[index]) = byteOrder;

    index += sizeof(uint32_t);

    return true;
}

bool PacketWriter::write(int8_t* buf, unsigned int len, unsigned int& index, const int8_t& value) {
    if (index + sizeof(int8_t) > len) {
        return false;
    }

    buf[index] = value;

    index += sizeof(int8_t);

    return true;
}

bool PacketWriter::write(int8_t* buf, unsigned int len, unsigned int& index, const int16_t& value) {
    if (index + sizeof(int16_t) > len) {
        return false;
    }

    int16_t byteOrder = htons(value);
    *reinterpret_cast<int16_t*>(&buf[index]) = byteOrder;

    index += sizeof(int16_t);

    return true;
}

bool PacketWriter::write(int8_t* buf, unsigned int len, unsigned int& index, const int32_t& value) {
    if (index + sizeof(int32_t) > len) {
        return false;
    }

    int32_t byteOrder = htonl(value);
    *reinterpret_cast<int32_t*>(&buf[index]) = byteOrder;

    index += sizeof(int32_t);

    return true;
}

}
}
