
set (UI_HPP
    manager.hpp
    texture.hpp
    renderqueue.hpp
    ingamewindowrenderer.hpp
    ingamewindow.hpp
    )

set (UI_CPP
    manager.cpp
    texture.cpp
    renderqueue.cpp
    ingamewindowrenderer.cpp
    ingamewindow.cpp
    )

add_library(uomeui STATIC ${UI_HPP} ${UI_CPP})

