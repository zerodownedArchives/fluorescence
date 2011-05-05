
#include "string.hpp"

#include "exception.hpp"
#include "logger.hpp"

#include <string.h>

namespace uome {

UnicodeString StringConverter::fromUtf8(const std::string& string) {
    UnicodeString ret = UnicodeString::fromUTF8(string);
    if (ret.isBogus()) {
        ret = UnicodeString("##UOMEERROR"); // set error string
        LOG_WARN(LOGTYPE_UNKNOWN, "Unable to convert from utf-8 string");
    }
    return ret;
}

UnicodeString StringConverter::fromUtf8(const char* buffer) {
    UnicodeString ret = UnicodeString::fromUTF8(buffer);
    if (ret.isBogus()) {
        ret = UnicodeString("##UOMEERROR"); // set error string
        LOG_WARN(LOGTYPE_UNKNOWN, "Unable to convert from utf-8 string");
    }
    return ret;
}

UnicodeString StringConverter::fromUtf8(const char* buffer, int bufferSize) {
    UnicodeString ret = UnicodeString::fromUTF8(StringPiece(buffer, bufferSize));
    if (ret.isBogus()) {
        ret = UnicodeString("##UOMEERROR"); // set error string
        LOG_WARN(LOGTYPE_UNKNOWN, "Unable to convert from utf-8 string");
    }
    return ret;
}

UnicodeString StringConverter::fromUnicode(const char* buffer, int bufferSize) {
    UErrorCode error = U_ZERO_ERROR;
    UnicodeString ret(buffer, bufferSize, getUnicodeConverter(), error);
    if (U_FAILURE(error)) {
        ret = UnicodeString("##UOMEERROR"); // set error string
        LOG_WARN(LOGTYPE_UNKNOWN, "Unable to convert from utf-16be string");
    }
    return ret;
}

UnicodeString StringConverter::fromNumber(unsigned int nr) {
    UnicodeString ret;
    unsigned int len = u_sprintf(ret.getBuffer(20), "%u", nr);
    ret.releaseBuffer(len);
    return ret;
}

UnicodeString StringConverter::fromNumber(int nr) {
    UnicodeString ret;
    unsigned int len = u_sprintf(ret.getBuffer(20), "%i", nr);
    ret.releaseBuffer(len);
    return ret;
}

int StringConverter::toUtf8(const UnicodeString& str, char* buffer, int bufferSize, bool nullTerminated) {
    std::string utf8Str = toUtf8String(str);

    // we want byte count, not real character count here, so length() is okay
    int ret = utf8Str.length();

    if (ret < bufferSize) {
        memcpy(buffer, utf8Str.c_str(), ret);
    } else {
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

std::string StringConverter::toUtf8String(const UnicodeString& str) {
    std::string ret;
    str.toUTF8String(ret);
    return ret;
}

int StringConverter::toUnicode(const UnicodeString& str, char* buffer, int bufferSize, bool nullTerminated) {
    UErrorCode error = U_ZERO_ERROR;
    int ret = str.extract(buffer, bufferSize, getUnicodeConverter(), error);
    if (U_FAILURE(error)) {
        LOG_WARN(LOGTYPE_UNKNOWN, "Unable to convert to utf-16be string");
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

UConverter* StringConverter::getUnicodeConverter() {
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

UnicodeString StringConverter::fromUtf8(const int8_t* buffer, int bufferSize) {
    return fromUtf8(reinterpret_cast<const char*>(buffer), bufferSize);
}

UnicodeString StringConverter::fromUnicode(const int8_t* buffer, int bufferSize) {
    return fromUnicode(reinterpret_cast<const char*>(buffer), bufferSize);
}

int StringConverter::toUtf8(const UnicodeString& str, int8_t* buffer, int bufferSize, bool nullTerminated) {
    return toUtf8(str, reinterpret_cast<char*>(buffer), bufferSize, nullTerminated);
}

int StringConverter::toUnicode(const UnicodeString& str, int8_t* buffer, int bufferSize, bool nullTerminated) {
    return toUnicode(str, reinterpret_cast<char*>(buffer), bufferSize, nullTerminated);
}

}
