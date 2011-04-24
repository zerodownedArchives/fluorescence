
#include "string.hpp"

#include "exception.hpp"

#include "logger.hpp"

namespace uome {

UnicodeString StringConverter::fromUtf8(const char* buffer, int bufferSize) {
    UErrorCode error = U_ZERO_ERROR;
    UnicodeString ret(buffer, bufferSize, getUtf8Converter(), error);
    if (U_FAILURE(error)) {
        ret = UnicodeString("");
        LOG_WARN(LOGTYPE_UNKNOWN, "Unable to convert from utf-8 string");
    }
    return ret;
}

UnicodeString StringConverter::fromUnicode(const char* buffer, int bufferSize) {
    UErrorCode error = U_ZERO_ERROR;
    UnicodeString ret(buffer, bufferSize, getUnicodeConverter(), error);
    if (U_FAILURE(error)) {
        ret = UnicodeString("");
        LOG_WARN(LOGTYPE_UNKNOWN, "Unable to convert from utf-16be string");
    }
    return ret;
}

int StringConverter::toUtf8(const UnicodeString& str, char* buffer, int bufferSize, bool nullTerminated) {
    UErrorCode error = U_ZERO_ERROR;
    int ret = str.extract(buffer, bufferSize, getUtf8Converter(), error);
    if (U_FAILURE(error)) {
        LOG_WARN(LOGTYPE_UNKNOWN, "Unable to convert to utf-8 string");
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

UConverter* StringConverter::getUtf8Converter() {
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
