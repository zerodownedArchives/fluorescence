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


#ifndef FLUO_DATA_EXCEPTION_HPP
#define FLUO_DATA_EXCEPTION_HPP

#include <exception>
#include <string>

#include "string.hpp"

namespace fluo {

class Exception : public std::exception {
public:
    Exception(const UnicodeString& str) throw() {
        msg_ = StringConverter::toUtf8String(str);
    }

    Exception(const UnicodeString& str, const UnicodeString& param) throw() {
        UnicodeString combined(str);
        combined.append("  ").append(param);
        msg_ = StringConverter::toUtf8String(combined);
    }

    Exception(const char* msg) throw() {
        msg_ = msg;
    }

    Exception(const char* msg, const char* param) throw() {
        msg_ = msg;
        msg_.append(param);
    }

    ~Exception() throw() {
    }

    const char* what() const throw() {
        return msg_.c_str();
    }

private:
    std::string msg_;
};

}


#endif
