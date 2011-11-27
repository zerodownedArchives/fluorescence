
set(MISC_HPP
    misc/log.hpp
    misc/string.hpp
    misc/variable.hpp
    misc/config.hpp
    misc/exception.hpp
    misc/random.hpp
    )

set(MISC_CPP
    misc/log.cpp
    misc/string.cpp
    misc/variable.cpp
    misc/config.cpp
    misc/random.cpp
    )

set(PUGIXML
    misc/pugixml/pugixml.hpp
    misc/pugixml/pugiconfig.hpp
    misc/pugixml/pugixml.cpp)

set(MISC_FILES ${MISC_HPP} ${MISC_CPP} ${PUGIXML} PARENT_SCOPE)

