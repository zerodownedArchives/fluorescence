#ifndef UOME_DATA_EXCEPTION_HPP
#define UOME_DATA_EXCEPTION_HPP

#include <exception>

namespace uome {

class Exception : public std::exception {
public:
    Exception(const char* msg) : msg_(msg) {
    }

    const char* what() const throw() {
        return msg_;
    }

private:
    const char* msg_;
};

}


#endif
