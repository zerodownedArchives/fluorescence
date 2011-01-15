
set (UI_HPP
    manager.hpp
    texture.hpp
    renderqueue.hpp
    ingameviewrenderer.hpp
    bitmask.hpp
    )

set (UI_CPP
    manager.cpp
    texture.cpp
    renderqueue.cpp
    ingameviewrenderer.cpp
    bitmask.cpp
    )

set (GUMPMENU_HPP
    gumpmenu.hpp
    gumpelement.hpp
    ingameview.hpp
    )

set (GUMPMENU_CPP
    gumpmenu.cpp
    gumpelement.cpp
    ingameview.cpp
    )

add_library(uomeui STATIC ${UI_HPP} ${UI_CPP} ${GUMPMENU_HPP} ${GUMPMENU_CPP})

