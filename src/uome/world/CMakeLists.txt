
set (WORLD_HPP
    manager.hpp
    ingameobject.hpp
    map.hpp
    statics.hpp
    sector.hpp
    sectormanager.hpp
    lightmanager.cpp
    )

set (WORLD_CPP
    manager.cpp
    ingameobject.cpp
    map.cpp
    statics.cpp
    sector.cpp
    sectormanager.cpp
    lightmanager.hpp
    )

add_library(uomeworld STATIC ${WORLD_HPP} ${WORLD_CPP})

#add_subdirectory(test)
