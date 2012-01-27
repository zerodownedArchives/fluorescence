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



#include "variable.hpp"

#include <sstream>

#include <misc/log.hpp>

namespace fluo {

Variable::Variable() : isDefault_(true), valueType_(TYPE_UNKNOWN) {
}

bool Variable::isInt() const {
    return valueType_ == TYPE_INT;
}

bool Variable::isBool() const {
    return valueType_ == TYPE_BOOL;
}

bool Variable::isString() const {
    return valueType_ == TYPE_STRING;
}

bool Variable::isPath() const {
    return valueType_ == TYPE_PATH;
}


bool Variable::isDefault() const {
    return isDefault_;
}


const bool Variable::asBool() const {
    if (isBool() || isInt()) {
        return valueBool_;
    }

    LOG_ERROR << "Unable to cast variable value " << asString() << " to bool" << std::endl;
    throw BadCastException();
}

const int Variable::asInt() const {
    if (isInt() || isBool()) {
        return valueInt_;
    }

    LOG_ERROR << "Unable to cast variable value " << asString() << " to int" << std::endl;
    throw BadCastException();
}

const UnicodeString& Variable::asString() const {
    if (isString() || isInt() || isPath() || isBool()) {
        return valueString_;
    }

    LOG_ERROR << "Unable to cast variable value to string" << std::endl;
    throw BadCastException();
}

std::string Variable::asUtf8() const {
    if (isString() || isPath()) {
        return StringConverter::toUtf8String(valueString_);
    }

    LOG_ERROR << "Unable to cast variable value " << asString() << " to utf" << std::endl;
    throw BadCastException();
}

const boost::filesystem::path& Variable::asPath() const {
    if (isPath() || isString()) {
        return valuePath_;
    }

    LOG_ERROR << "Unable to cast variable value " << asString() << " to path" << std::endl;
    throw BadCastException();
}

void Variable::setBool(bool val, bool isDefault) {
    valueBool_ = val;
    isDefault_ = isDefault;
    valueType_ = TYPE_BOOL;

    valueString_ = StringConverter::fromNumber(valueBool_);
    valueInt_ = val;
}

void Variable::setInt(int val, bool isDefault) {
    valueInt_ = val;
    isDefault_ = isDefault;
    valueType_ = TYPE_INT;

    valueString_ = StringConverter::fromNumber(valueInt_);
    valueBool_ = val != 0;
}

void Variable::setString(const UnicodeString& val, bool isDefault) {
    valueString_ = val;
    isDefault_ = isDefault;
    valueType_ = TYPE_STRING;

    std::string utf8Str = StringConverter::toUtf8String(val);
    valuePath_ = utf8Str;
}

void Variable::setPath(const boost::filesystem::path& val, bool isDefault) {
    valuePath_ = val;
    isDefault_ = isDefault;
    valueType_ = TYPE_PATH;

    valueString_ = StringConverter::fromUtf8(valuePath_.string());
}

bool Variable::parse(const char* str) {
    std::istringstream iss(str);

    int nr;
    iss >> nr;

    if (!iss.fail()) {
        setInt(nr);
        return true;
    }

    iss.clear();
    iss.seekg(std::ios::beg);

    bool b;
    iss >> std::boolalpha >> b;

    if (!iss.fail()) {
        setBool(b);
        return true;
    }

    setString(str);

    return true;
}

unsigned int Variable::valueType() const {
    return valueType_;
}

std::vector<int> Variable::asIntList() const {
    if (!isString()) {
        throw BadCastException();
    }

    std::string str = StringConverter::toUtf8String(valueString_);
    std::vector<int> vect;

    std::stringstream ss(str);

    int i;

    while (ss >> i) {
        vect.push_back(i);

        while (ss.peek() == ',' || ss.peek() == ' ') {
            ss.ignore();
        }
    }

    return vect;
}

}
