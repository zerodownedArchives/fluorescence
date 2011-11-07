#ifndef FLUO_NET_PACKETREADER_HPP
#define FLUO_NET_PACKETREADER_HPP

#include <misc/string.hpp>

namespace fluo {
namespace net {

class PacketReader {
public:
    static bool readUnicodeNull(const int8_t* buf, unsigned int len, unsigned int& index, UnicodeString& value);
    static bool readUnicodeFixed(const int8_t* buf, unsigned int len, unsigned int& index, UnicodeString& value, unsigned int fixed);

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
