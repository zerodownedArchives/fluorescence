#ifndef UOME_STRING_HPP
#define UOME_STRING_HPP

#include "exception.hpp"

#include <unicode/unistr.h>
extern "C" {
#include <unicode/ustdio.h>
}

namespace uome {

class StringConverter {
public:
    static UnicodeString fromUtf8(const char* buffer, int bufferSize);
    static UnicodeString fromUnicode(const char* buffer, int bufferSize);
    static int toUtf8(const UnicodeString& str, char* buffer, int bufferSize, bool nullTerminated);
    static int toUnicode(const UnicodeString& str, char* buffer, int bufferSize, bool nullTerminated);

    static UnicodeString fromUtf8(const int8_t* buffer, int bufferSize);
    static UnicodeString fromUnicode(const int8_t* buffer, int bufferSize);
    static int toUtf8(const UnicodeString& str, int8_t* buffer, int bufferSize, bool nullTerminated);
    static int toUnicode(const UnicodeString& str, int8_t* buffer, int bufferSize, bool nullTerminated);

private:
    static UConverter* getUtf8Converter();
    static UConverter* getUnicodeConverter();
};

}


#endif
