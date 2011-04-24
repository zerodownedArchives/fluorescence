
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

//s32 cPacketWriter::writeUnicodeFixed(u8* _buf, s32 _buflen, s32* _index,
        //cString* _value, s32 _strlen) {
    //if (*_index + _strlen < _buflen) {
//#ifdef _MSC_VER
        //u8* strbuf = new u8[_strlen];
//#else
        //u8 strbuf[_strlen];
//#endif
        //memset(&strbuf, 0, _strlen);

        //s32 bytes = etc::cStringConverter::getSingleton()->stringToUnicodeBytes(_value, (u8*)&strbuf, _strlen, false);
        //cPacketWriter::writeU8Arr(_buf, _buflen, _index, (u8*)&strbuf, _strlen);
//#ifdef _MSC_VER
        //delete[] strbuf;
//#endif
        //if (bytes == _strlen)
            //return _strlen;
        //else
            //return bytes;
    //} else
        //return -1;
//}

}
}
