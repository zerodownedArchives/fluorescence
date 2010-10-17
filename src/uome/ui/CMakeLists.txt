
set (UI_HPP
    manager.hpp
    texture.hpp
    )

set (UI_CPP
    manager.cpp
    texture.cpp
    )

add_library(uomeui STATIC ${UI_HPP} ${UI_CPP})

add_subdirectory(test)
