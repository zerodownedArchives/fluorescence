#ifndef UOME_NET_PACKETWRITER_HPP
#define UOME_NET_PACKETWRITER_HPP

#include <misc/string.hpp>

namespace uome {
namespace net {

class PacketWriter {
public:
    template<typename T>
    static bool write(int8_t* buf, unsigned int len, unsigned int& index, const T& value) {
        if (index + sizeof(T) > len) {
            return false;
        }

        *reinterpret_cast<T*>(&buf[index]) = value;

        index += sizeof(T);

        return true;
    }

    static bool writeUnicodeNull(int8_t* buf, unsigned int len, unsigned int& index, const UnicodeString& value);
    static bool writeUnicodeFixed(int8_t* buf, unsigned int len, unsigned int& index, const UnicodeString& value, unsigned int fixed);

    static bool writeUtf8Null(int8_t* buf, unsigned int len, unsigned int& index, const UnicodeString& value);
    static bool writeUtf8Fixed(int8_t* buf, unsigned int len, unsigned int& index, const UnicodeString& value, unsigned int fixed);

private:
    static int8_t* fixedStringBuf_;
};

}
}


#endif
