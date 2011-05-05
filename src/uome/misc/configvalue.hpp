#ifndef UOME_MISC_CONFIGVALUE_HPP
#define UOME_MISC_CONFIGVALUE_HPP

#include <boost/filesystem/path.hpp>
#include <misc/string.hpp>

namespace uome {

class ConfigValue {
public:
    class BadCastException : public std::exception {
    public:
        const char* what() const throw() {
            return "Bad config value cast";
        }
    };

    static const unsigned int TYPE_UNKNOWN = 0;
    static const unsigned int TYPE_UINT = 1;
    static const unsigned int TYPE_INT = 2;
    static const unsigned int TYPE_BOOL = 3;
    static const unsigned int TYPE_STRING = 4;
    static const unsigned int TYPE_PATH = 5;

    ConfigValue();

    bool isUint();
    bool isInt();
    bool isBool();
    bool isString();
    bool isPath();

    bool isDefault();

    void parse(const char* str);

    const bool asBool() const;
    const unsigned int asUint() const;
    const int asInt() const;
    const UnicodeString& asString() const;
    std::string asUtf8() const;
    const boost::filesystem::path& asPath() const;

    void setBool(bool val, bool isDefault = false);
    void setUint(unsigned int val, bool isDefault = false);
    void setInt(int val, bool isDefault = false);
    void setString(const UnicodeString& val, bool isDefault = false);
    void setPath(const boost::filesystem::path& val, bool isDefault = false);
    void setPathFromUString(const UnicodeString& val, bool isDefault = false);

private:
    bool isDefault_;
    unsigned int valueType_;

    unsigned int valueUint_;
    int valueInt_;
    bool valueBool_;
    UnicodeString valueString_;
    boost::filesystem::path valuePath_;
};

}

#endif
