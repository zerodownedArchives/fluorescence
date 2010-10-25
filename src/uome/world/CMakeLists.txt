
set (WORLD_HPP
    map.hpp
    ingameobject.hpp
    )

set (WORLD_CPP
    ingameobject.cpp
    )

add_library(uomeworld STATIC ${WORLD_HPP} ${WORLD_CPP})

#add_subdirectory(test)
