#ifndef UOME_NET_PACKETREADER_HPP
#define UOME_NET_PACKETREADER_HPP

#include <misc/string.hpp>

namespace uome {
namespace net {

class PacketReader {
public:
    template<typename T>
    static bool read(const int8_t* buf, unsigned int len, unsigned int& index, T& value) {
        if (index + sizeof(T) > len) {
            return false;
        }

        value = *reinterpret_cast<const T*>(&buf[index]);

        index += sizeof(T);

        return true;
    }

    static bool readUnicodeNull(const int8_t* buf, unsigned int len, unsigned int& index, UnicodeString& value);
    static bool readUnicodeFixed(const int8_t* buf, unsigned int len, unsigned int& index, UnicodeString& value, unsigned int fixed);

    static bool readUtf8Null(const int8_t* buf, unsigned int len, unsigned int& index, UnicodeString& value);
    static bool readUtf8Fixed(const int8_t* buf, unsigned int len, unsigned int& index, UnicodeString& value, unsigned int fixed);

private:
    static int8_t* fixedStringBuf_;
};

}
}


#endif
