#ifndef UOME_DATA_EXCEPTION_HPP
#define UOME_DATA_EXCEPTION_HPP

#include <exception>
#include <string>

namespace uome {

class Exception : public std::exception {
public:
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
