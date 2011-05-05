
#include "configvalue.hpp"

#include <sstream>

namespace uome {

ConfigValue::ConfigValue() : isDefault_(true), valueType_(TYPE_UNKNOWN) {
}

bool ConfigValue::isInt() const {
    return valueType_ == TYPE_INT;
}

bool ConfigValue::isBool() const {
    return valueType_ == TYPE_BOOL;
}

bool ConfigValue::isString() const {
    return valueType_ == TYPE_STRING;
}

bool ConfigValue::isPath() const {
    return valueType_ == TYPE_PATH;
}


bool ConfigValue::isDefault() const {
    return isDefault_;
}


const bool ConfigValue::asBool() const {
    if (isBool() || isInt()) {
        return valueBool_;
    }
    throw BadCastException();
}

const int ConfigValue::asInt() const {
    if (isInt() || isBool()) {
        return valueInt_;
    }
    throw BadCastException();
}

const UnicodeString& ConfigValue::asString() const {
    if (isString() || isInt() || isPath() || isBool()) {
        return valueString_;
    }
    throw BadCastException();
}

std::string ConfigValue::asUtf8() const {
    if (isString() || isPath()) {
        return StringConverter::toUtf8String(valueString_);
    }
    throw BadCastException();
}

const boost::filesystem::path& ConfigValue::asPath() const {
    if (isPath() || isString()) {
        return valuePath_;
    }
    throw BadCastException();
}

void ConfigValue::setBool(bool val, bool isDefault) {
    valueBool_ = val;
    isDefault_ = isDefault;
    valueType_ = TYPE_BOOL;

    valueString_ = StringConverter::fromNumber(valueBool_);
    valueInt_ = val;
}

void ConfigValue::setInt(int val, bool isDefault) {
    valueInt_ = val;
    isDefault_ = isDefault;
    valueType_ = TYPE_INT;

    valueString_ = StringConverter::fromNumber(valueInt_);
    valueBool_ = val != 0;
}

void ConfigValue::setString(const UnicodeString& val, bool isDefault) {
    valueString_ = val;
    isDefault_ = isDefault;
    valueType_ = TYPE_STRING;

    std::string utf8Str = StringConverter::toUtf8String(val);
    valuePath_ = utf8Str;
}

void ConfigValue::setPath(const boost::filesystem::path& val, bool isDefault) {
    valuePath_ = val;
    isDefault_ = isDefault;
    valueType_ = TYPE_PATH;

    valueString_ = StringConverter::fromUtf8(valuePath_.string());
}

bool ConfigValue::parse(const char* str) {
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

unsigned int ConfigValue::valueType() const {
    return valueType_;
}

}
