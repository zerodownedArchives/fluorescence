#ifndef FLUO_MISC_VARIABLE_HPP
#define FLUO_MISC_VARIABLE_HPP

#include <vector>
#include <boost/filesystem/path.hpp>
#include <misc/string.hpp>

namespace fluo {

class Variable {
public:
    class BadCastException : public std::exception {
    public:
        const char* what() const throw() {
            return "Bad config value cast";
        }
    };

    static const unsigned int TYPE_UNKNOWN = 0;
    static const unsigned int TYPE_INT = 1;
    static const unsigned int TYPE_BOOL = 2;
    static const unsigned int TYPE_STRING = 3;
    static const unsigned int TYPE_PATH = 4;

    Variable();

    bool isInt() const;
    bool isBool() const;
    bool isString() const;
    bool isPath() const;

    bool isDefault() const;

    bool parse(const char* str);

    const bool asBool() const;
    const int asInt() const;
    const UnicodeString& asString() const;
    std::string asUtf8() const;
    const boost::filesystem::path& asPath() const;

    void setBool(bool val, bool isDefault = false);
    void setInt(int val, bool isDefault = false);
    void setString(const UnicodeString& val, bool isDefault = false);
    void setPath(const boost::filesystem::path& val, bool isDefault = false);

    unsigned int valueType() const;

    std::vector<int> asIntList() const;

private:
    bool isDefault_;
    unsigned int valueType_;

    int valueInt_;
    bool valueBool_;
    UnicodeString valueString_;
    boost::filesystem::path valuePath_;
};

}

#endif
