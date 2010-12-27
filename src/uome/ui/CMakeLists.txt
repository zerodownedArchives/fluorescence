
set (UI_HPP
    manager.hpp
    texture.hpp
    renderqueue.hpp
    renderer.hpp
    )

set (UI_CPP
    manager.cpp
    texture.cpp
    renderqueue.cpp
    renderer.cpp
    )

add_library(uomeui STATIC ${UI_HPP} ${UI_CPP})

