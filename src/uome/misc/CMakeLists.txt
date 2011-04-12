
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

set(PUGIXML
    pugixml.hpp
    pugiconfig.hpp
    pugixml.cpp)

add_library(uomemisc STATIC
    ${MISC_HPP} ${MISC_CPP} ${PUGIXML} )

