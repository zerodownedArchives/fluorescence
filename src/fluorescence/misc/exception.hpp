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
