
set(MISC_HPP
    logger.hpp
    string.hpp
    config.hpp
    exception.hpp
    )

set(MISC_CPP
    logger.cpp
    string.cpp
    config.cpp
    )

add_library(uomemisc STATIC
    ${MISC_HPP} ${MISC_CPP})

