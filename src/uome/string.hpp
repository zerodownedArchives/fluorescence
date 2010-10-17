#ifndef UOME_STRING_HPP
#define UOME_STRING_HPP

//#include "logger.hpp"
#include "exception.hpp"

#include <unicode/unistr.h>
extern "C" {
#include <unicode/ustdio.h>
}

namespace uome {

typedef UnicodeString String;

class StringConverter {
public:
    static String fromUtf8(const char* buffer, int bufferSize) {
        UErrorCode error = U_ZERO_ERROR;
        String ret(buffer, bufferSize, getUtf8Converter(), error);
        if (U_FAILURE(error)) {
            ret = String("");
            //LOG_WARN(LOGTYPE_UNKNOWN, "Unable to convert from utf-8 string");
        }
        return ret;
    }

    static String fromUnicode(const char* buffer, int bufferSize) {
        UErrorCode error = U_ZERO_ERROR;
        String ret(buffer, bufferSize, getUnicodeConverter(), error);
        if (U_FAILURE(error)) {
            ret = String("");
            //LOG_WARN(LOGTYPE_UNKNOWN, "Unable to convert from utf-16be string");
        }
        return ret;
    }

    static int toUtf8(const String& str, char* buffer, int bufferSize, bool nullTerminated) {
        UErrorCode error = U_ZERO_ERROR;
        int ret = str.extract(buffer, bufferSize, getUtf8Converter(), error);
        if (U_FAILURE(error)) {
            //LOG_WARN(LOGTYPE_UNKNOWN, "Unable to convert to utf-8 string");
            return -1;
        }

        if (!nullTerminated) {
            return ret;
        } else {
            if (ret+1 < bufferSize) {
                buffer[ret++] = 0;

                return ret;
            } else {
                return -1;
            }
        }
    }

    static int toUnicode(const String& str, char* buffer, int bufferSize, bool nullTerminated) {
        UErrorCode error = U_ZERO_ERROR;
        int ret = str.extract(buffer, bufferSize, getUnicodeConverter(), error);
        if (U_FAILURE(error)) {
            //LOG_WARN(LOGTYPE_UNKNOWN, "Unable to convert to utf-16be string");
            return -1;
        }

        if (!nullTerminated) {
            return ret;
        } else {
            if (ret+2 < bufferSize) {
                buffer[ret++] = 0;
                buffer[ret++] = 0;

                return ret;
            } else {
                return -1;
            }
        }
    }

private:
    static UConverter* getUtf8Converter() {
        static UConverter* conv = NULL;
        if (!conv) {
            UErrorCode error = U_ZERO_ERROR;
            conv = ucnv_open("UTF-8", &error);
            if (U_FAILURE(error)) {
                throw Exception("Error creating UTF-8 converter");
            }
        }

        return conv;
    }

    static UConverter* getUnicodeConverter() {
        static UConverter* conv = NULL;
        if (!conv) {
            UErrorCode error = U_ZERO_ERROR;
            conv = ucnv_open("UTF-16BE", &error);
            if (U_FAILURE(error)) {
                throw Exception("Error creating UTF-16BE converter");
            }
        }

        return conv;
    }
};

}


#endif
