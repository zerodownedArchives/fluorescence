#ifndef UOME_NET_PACKETWRITER_HPP
#define UOME_NET_PACKETWRITER_HPP

#include <misc/string.hpp>

namespace uome {
namespace net {

class PacketWriter {
public:
    static bool writeUnicodeNull(int8_t* buf, unsigned int len, unsigned int& index, const UnicodeString& value);
    static bool writeUnicodeFixed(int8_t* buf, unsigned int len, unsigned int& index, const UnicodeString& value, unsigned int fixed);

    static bool writeUtf8Null(int8_t* buf, unsigned int len, unsigned int& index, const UnicodeString& value);
    static bool writeUtf8Fixed(int8_t* buf, unsigned int len, unsigned int& index, const UnicodeString& value, unsigned int fixed);
    static bool writeUtf8Null(int8_t* buf, unsigned int len, unsigned int& index, const std::string& value);
    static bool writeUtf8Fixed(int8_t* buf, unsigned int len, unsigned int& index, const std::string& value, unsigned int fixed);

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
