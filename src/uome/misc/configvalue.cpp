
#include "configvalue.hpp"

namespace uome {

ConfigValue::ConfigValue() : isDefault_(true), valueType_(TYPE_UNKNOWN) {
}

bool ConfigValue::isUint() {
    return valueType_ == TYPE_UINT;
}

bool ConfigValue::isInt() {
    return valueType_ == TYPE_INT;
}

bool ConfigValue::isBool() {
    return valueType_ == TYPE_BOOL;
}

bool ConfigValue::isString() {
    return valueType_ == TYPE_STRING;
}

bool ConfigValue::isPath() {
    return valueType_ == TYPE_PATH;
}


bool ConfigValue::isDefault() {
    return isDefault_;
}


const bool ConfigValue::asBool() const {
    if (valueType_ == TYPE_BOOL) {
        return valueBool_;
    }
    throw BadCastException();
}

const unsigned int ConfigValue::asUint() const {
    if (valueType_ == TYPE_UINT) {
        return valueUint_;
    }
    throw BadCastException();
}

const int ConfigValue::asInt() const {
    if (valueType_ == TYPE_INT) {
        return valueInt_;
    }
    throw BadCastException();
}

const UnicodeString& ConfigValue::asString() const {
    if (valueType_ == TYPE_STRING) {
        return valueString_;
    }
    throw BadCastException();
}

std::string ConfigValue::asUtf8() const {
    if (valueType_ == TYPE_STRING) {
        return StringConverter::toUtf8String(valueString_);
    }
    throw BadCastException();
}

const boost::filesystem::path& ConfigValue::asPath() const {
    if (valueType_ == TYPE_PATH) {
        return valuePath_;
    }
    throw BadCastException();
}

void ConfigValue::setBool(bool val, bool isDefault) {
    valueBool_ = val;
    isDefault_ = isDefault;
    valueType_ = TYPE_BOOL;
}

void ConfigValue::setUint(unsigned int val, bool isDefault) {
    valueUint_ = val;
    isDefault_ = isDefault;
    valueType_ = TYPE_UINT;
}

void ConfigValue::setInt(int val, bool isDefault) {
    valueInt_ = val;
    isDefault_ = isDefault;
    valueType_ = TYPE_INT;
}

void ConfigValue::setString(const UnicodeString& val, bool isDefault) {
    valueString_ = val;
    isDefault_ = isDefault;
    valueType_ = TYPE_STRING;
}

void ConfigValue::setPath(const boost::filesystem::path& val, bool isDefault) {
    valuePath_ = val;
    isDefault_ = isDefault;
    valueType_ = TYPE_PATH;
}

void ConfigValue::setPathFromUString(const UnicodeString& val, bool isDefault) {
    std::string utf8Path = StringConverter::toUtf8String(val);
    valuePath_ = utf8Path;
    isDefault_ = isDefault;
    valueType_ = TYPE_PATH;
}

}
