
set(MISC_HPP
    misc/logger.hpp
    misc/string.hpp
    misc/config.hpp
    misc/exception.hpp
    )

set(MISC_CPP
    misc/logger.cpp
    misc/string.cpp
    misc/config.cpp
    )

set(PUGIXML
    misc/pugixml.hpp
    misc/pugiconfig.hpp
    misc/pugixml.cpp)

set(MISC_FILES ${MISC_HPP} ${MISC_CPP} ${PUGIXML} PARENT_SCOPE)

