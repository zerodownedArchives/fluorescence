
#include "string.hpp"

#include "exception.hpp"
#include "log.hpp"

#include <string.h>

namespace fluo {

UnicodeString StringConverter::fromUtf8(const std::string& string) {
    UnicodeString ret = UnicodeString::fromUTF8(string);
    if (ret.isBogus()) {
        ret = UnicodeString("##FLUOERROR"); // set error string
        LOG_WARN << "Unable to convert from utf-8 string" << std::endl;
    }
    return ret;
}

UnicodeString StringConverter::fromUtf8(const char* buffer) {
    UnicodeString ret = UnicodeString::fromUTF8(buffer);
    if (ret.isBogus()) {
        ret = UnicodeString("##FLUOERROR"); // set error string
        LOG_WARN << "Unable to convert from utf-8 string" << std::endl;
    }
    return ret;
}

UnicodeString StringConverter::fromUtf8(const char* buffer, int bufferSize) {
    UnicodeString tmp = UnicodeString::fromUTF8(StringPiece(buffer, bufferSize));

    // a little strange, but otherwise a unicode string might be returned that is just filled with 0x00, but still
    // has length > 0 and is != "" etc
    UnicodeString ret(tmp.getBuffer());
    if (ret.isBogus()) {
        ret = UnicodeString("##FLUOERROR"); // set error string
        LOG_WARN << "Unable to convert from utf-8 string" << std::endl;
    }
    return ret;
}

UnicodeString StringConverter::fromUnicode(const char* buffer, int bufferSize) {
    UErrorCode error = U_ZERO_ERROR;
    UnicodeString tmp(buffer, bufferSize, getUnicodeConverter(), error);
    if (U_FAILURE(error)) {
        tmp = UnicodeString("##FLUOERROR"); // set error string
        LOG_WARN << "Unable to convert from utf-16-be string" << std::endl;
    }
    UnicodeString ret(tmp.getBuffer());
    return ret;
}

UnicodeString StringConverter::fromNumber(int nr) {
    UnicodeString ret;
    getNumberFormat()->format(nr, ret);
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
        LOG_WARN << "Unable to convert to utf-16-be string" << std::endl;
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

int StringConverter::toInt(const UnicodeString& str) {
    UErrorCode error = U_ZERO_ERROR;
    Formattable fmt(-999);
    getNumberFormat()->parse(str, fmt, error);

    int ret = 0;
    if (U_FAILURE(error) || !fmt.isNumeric()) {
        LOG_ERROR << "Error parsing number from string: " << str << std::endl;
    } else {
        ret = fmt.getLong();
    }

    return ret;
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

NumberFormat* StringConverter::getNumberFormat() {
    static NumberFormat* nf = NULL;
    if (!nf) {
        UErrorCode errorCode = U_ZERO_ERROR;
        nf = NumberFormat::createInstance(errorCode);

        if (U_FAILURE(errorCode)) {
            throw Exception("Error creating unicode number format");
        }

        nf->setGroupingUsed(false);
    }

    return nf;
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
