
set (WORLD_HPP
    maptile.hpp
    uiexception.hpp
    )

set (WORLD_CPP
    manager.cpp
    uiexception.cpp
    )

add_library(uomeworld STATIC ${WORLD_HPP} ${WORLD_CPP})

#add_subdirectory(test)
