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


#ifndef FLUO_STRING_HPP
#define FLUO_STRING_HPP

#include <unicode/unistr.h>
#include <unicode/numfmt.h>
extern "C" {
#include <unicode/ustdio.h>
}

#include <boost/thread/mutex.hpp>

namespace fluo {

class StringConverter {
public:
    static UnicodeString fromUtf8(const std::string& string);
    static UnicodeString fromUtf8(const char* buffer);
    static UnicodeString fromUtf8(const char* buffer, int bufferSize);
    static UnicodeString fromUnicode(const char* buffer, int bufferSize);
    static UnicodeString fromUnicodeLE(const char* buffer, int bufferSize);

    static UnicodeString fromNumber(int nr);

    static int toUtf8(const UnicodeString& str, char* buffer, int bufferSize, bool nullTerminated);
    static int toUnicode(const UnicodeString& str, char* buffer, int bufferSize, bool nullTerminated);
    static int toUnicodeLE(const UnicodeString& str, char* buffer, int bufferSize, bool nullTerminated);

    static std::string toUtf8String(const UnicodeString& str);

    static UnicodeString fromUtf8(const int8_t* buffer, int bufferSize);
    static UnicodeString fromUnicode(const int8_t* buffer, int bufferSize);
    static UnicodeString fromUnicodeLE(const int8_t* buffer, int bufferSize);
    static int toUtf8(const UnicodeString& str, int8_t* buffer, int bufferSize, bool nullTerminated);
    static int toUnicode(const UnicodeString& str, int8_t* buffer, int bufferSize, bool nullTerminated);
    static int toUnicodeLE(const UnicodeString& str, int8_t* buffer, int bufferSize, bool nullTerminated);

    static int toInt(const UnicodeString& str, bool noErrorMessage = false);

private:
    static UConverter* getUnicodeConverter();
    static UConverter* getUnicodeConverterLE();
    static NumberFormat* getNumberFormat();

    static boost::mutex* unicodeConverterMutex_;
    static boost::mutex* unicodeConverterLeMutex_;
};

}


#endif
