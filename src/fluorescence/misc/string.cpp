/*
 * fluorescence is a free, customizable Ultima Online client.
 * Copyright (C) 2011-2012, http://fluorescence-client.org

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */



#include "string.hpp"

#include "exception.hpp"
#include "log.hpp"

#include <string.h>

namespace fluo {

boost::mutex* StringConverter::unicodeConverterMutex_ = new boost::mutex();
boost::mutex* StringConverter::unicodeConverterLeMutex_ = new boost::mutex;

UnicodeString StringConverter::fromUtf8(const std::string& string) {
    UnicodeString ret(string.c_str());
    // this does not work in the windows debug build: UnicodeString::fromUTF8(string);
    // although it looks "more correct" to me... 
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
    UnicodeString ret(tmp.getTerminatedBuffer());
    if (ret.isBogus()) {
        ret = UnicodeString("##FLUOERROR"); // set error string
        LOG_WARN << "Unable to convert from utf-8 string" << std::endl;
    }
    return ret;
}

UnicodeString StringConverter::fromUnicode(const char* buffer, int bufferSize) {
    boost::mutex::scoped_lock myLock(*unicodeConverterMutex_);
    UErrorCode error = U_ZERO_ERROR;
    UnicodeString tmp(buffer, bufferSize, getUnicodeConverter(), error);
    if (U_FAILURE(error)) {
        tmp = UnicodeString("##FLUOERROR"); // set error string
        LOG_WARN << "Unable to convert from utf-16-be string" << std::endl;
    }
    UnicodeString ret(tmp.getTerminatedBuffer());
    return ret;
}

UnicodeString StringConverter::fromUnicodeLE(const char* buffer, int bufferSize) {
    boost::mutex::scoped_lock myLock(*unicodeConverterLeMutex_);
    UErrorCode error = U_ZERO_ERROR;
    UnicodeString tmp(buffer, bufferSize, getUnicodeConverterLE(), error);
    if (U_FAILURE(error)) {
        tmp = UnicodeString("##FLUOERROR"); // set error string
        LOG_WARN << "Unable to convert from utf-16-le string" << std::endl;
    }
    UnicodeString ret(tmp.getTerminatedBuffer());
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
    boost::mutex::scoped_lock myLock(*unicodeConverterMutex_);
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

int StringConverter::toUnicodeLE(const UnicodeString& str, char* buffer, int bufferSize, bool nullTerminated) {
    boost::mutex::scoped_lock myLock(*unicodeConverterLeMutex_);
    UErrorCode error = U_ZERO_ERROR;
    int ret = str.extract(buffer, bufferSize, getUnicodeConverterLE(), error);
    if (U_FAILURE(error)) {
        LOG_WARN << "Unable to convert to utf-16-le string" << std::endl;
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

UConverter* StringConverter::getUnicodeConverterLE() {
    static UConverter* conv = NULL;
    if (!conv) {
        UErrorCode error = U_ZERO_ERROR;
        conv = ucnv_open("UTF-16LE", &error);
        if (U_FAILURE(error)) {
            throw Exception("Error creating UTF-16LE converter");
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

UnicodeString StringConverter::fromUnicodeLE(const int8_t* buffer, int bufferSize) {
    return fromUnicodeLE(reinterpret_cast<const char*>(buffer), bufferSize);
}

int StringConverter::toUtf8(const UnicodeString& str, int8_t* buffer, int bufferSize, bool nullTerminated) {
    return toUtf8(str, reinterpret_cast<char*>(buffer), bufferSize, nullTerminated);
}

int StringConverter::toUnicode(const UnicodeString& str, int8_t* buffer, int bufferSize, bool nullTerminated) {
    return toUnicode(str, reinterpret_cast<char*>(buffer), bufferSize, nullTerminated);
}

int StringConverter::toUnicodeLE(const UnicodeString& str, int8_t* buffer, int bufferSize, bool nullTerminated) {
    return toUnicodeLE(str, reinterpret_cast<char*>(buffer), bufferSize, nullTerminated);
}

}
