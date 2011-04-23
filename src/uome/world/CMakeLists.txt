
set (WORLD_HPP
    manager.hpp
    ingameobject.hpp
    map.hpp
    statics.hpp
    sector.hpp
    sectormanager.hpp
    lightmanager.hpp
    serverobject.hpp
    mobile.hpp
    )

set (WORLD_CPP
    manager.cpp
    ingameobject.cpp
    map.cpp
    statics.cpp
    sector.cpp
    sectormanager.cpp
    lightmanager.cpp
    serverobject.cpp
    mobile.cpp
    )

add_library(uomeworld STATIC ${WORLD_HPP} ${WORLD_CPP})

#add_subdirectory(test)
