
set (WORLD_HPP
    ingameobject.hpp
    map.hpp
    statics.hpp
    sector.hpp
    )

set (WORLD_CPP
    ingameobject.cpp
    map.cpp
    statics.cpp
    sector.cpp
    )

add_library(uomeworld STATIC ${WORLD_HPP} ${WORLD_CPP})

#add_subdirectory(test)
