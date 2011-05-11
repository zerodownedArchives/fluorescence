
set(MISC_HPP
    misc/log.hpp
    misc/string.hpp
    misc/configvalue.hpp
    misc/config.hpp
    misc/exception.hpp
    misc/filenamecaseconverter.hpp
    )

set(MISC_CPP
    misc/log.cpp
    misc/string.cpp
    misc/configvalue.cpp
    misc/config.cpp
    misc/filenamecaseconverter.cpp
    )

set(PUGIXML
    misc/pugixml/pugixml.hpp
    misc/pugixml/pugiconfig.hpp
    misc/pugixml/pugixml.cpp)

set(MISC_FILES ${MISC_HPP} ${MISC_CPP} ${PUGIXML} PARENT_SCOPE)

