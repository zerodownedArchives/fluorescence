
#include "variable.hpp"

#include <sstream>

namespace uome {

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
    throw BadCastException();
}

const int Variable::asInt() const {
    if (isInt() || isBool()) {
        return valueInt_;
    }
    throw BadCastException();
}

const UnicodeString& Variable::asString() const {
    if (isString() || isInt() || isPath() || isBool()) {
        return valueString_;
    }
    throw BadCastException();
}

std::string Variable::asUtf8() const {
    if (isString() || isPath()) {
        return StringConverter::toUtf8String(valueString_);
    }
    throw BadCastException();
}

const boost::filesystem::path& Variable::asPath() const {
    if (isPath() || isString()) {
        return valuePath_;
    }
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

}
