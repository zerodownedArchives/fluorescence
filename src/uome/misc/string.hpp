#ifndef UOME_STRING_HPP
#define UOME_STRING_HPP

#include "exception.hpp"

#include <unicode/unistr.h>
extern "C" {
#include <unicode/ustdio.h>
}

namespace uome {

typedef UnicodeString String;

class StringConverter {
public:
    static String fromUtf8(const char* buffer, int bufferSize);
    static String fromUnicode(const char* buffer, int bufferSize);
    static int toUtf8(const String& str, char* buffer, int bufferSize, bool nullTerminated);
    static int toUnicode(const String& str, char* buffer, int bufferSize, bool nullTerminated);

private:
    static UConverter* getUtf8Converter();
    static UConverter* getUnicodeConverter();
};

}


#endif
