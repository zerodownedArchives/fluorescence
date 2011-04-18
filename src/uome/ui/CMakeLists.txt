
set (UI_HPP
    manager.hpp
    texture.hpp
    animation.hpp
    textureprovider.hpp
    singletextureprovider.hpp
    animdatatextureprovider.hpp
    animtextureprovider.hpp
    renderqueue.hpp
    ingameviewrenderer.hpp
    bitmask.hpp
    cursormanager.hpp
    cursorimage.hpp
    doubleclickhandler.hpp
    )

set (UI_CPP
    manager.cpp
    texture.cpp
    animation.cpp
    singletextureprovider.cpp
    animdatatextureprovider.cpp
    animtextureprovider.cpp
    renderqueue.cpp
    ingameviewrenderer.cpp
    bitmask.cpp
    cursormanager.cpp
    cursorimage.cpp
    doubleclickhandler.cpp
    )

set (GUMPMENU_HPP
    gumpfactory.hpp
    gumpmenu.hpp
    gumpelement.hpp
    ingameview.hpp

    components/basebutton.hpp
    )

set (GUMPMENU_CPP
    gumpfactory.cpp
    gumpmenu.cpp
    gumpelement.cpp
    ingameview.cpp

    components/basebutton.cpp
    )

add_library(uomeui STATIC ${UI_HPP} ${UI_CPP} ${GUMPMENU_HPP} ${GUMPMENU_CPP})

