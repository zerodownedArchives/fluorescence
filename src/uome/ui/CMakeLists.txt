
set (UI_HPP
    ui/manager.hpp
    ui/texture.hpp
    ui/animation.hpp
    ui/textureprovider.hpp
    ui/singletextureprovider.hpp
    ui/animdatatextureprovider.hpp
    ui/animtextureprovider.hpp
    ui/renderqueue.hpp
    ui/ingameviewrenderer.hpp
    ui/bitmask.hpp
    ui/cursormanager.hpp
    ui/cursorimage.hpp
    ui/doubleclickhandler.hpp
    )

set (UI_CPP
    ui/manager.cpp
    ui/texture.cpp
    ui/animation.cpp
    ui/singletextureprovider.cpp
    ui/animdatatextureprovider.cpp
    ui/animtextureprovider.cpp
    ui/renderqueue.cpp
    ui/ingameviewrenderer.cpp
    ui/bitmask.cpp
    ui/cursormanager.cpp
    ui/cursorimage.cpp
    ui/doubleclickhandler.cpp
    )

set (GUMPMENU_HPP
    ui/gumpfactory.hpp
    ui/gumpmenu.hpp
    ui/gumpelement.hpp
    ui/ingameview.hpp

    ui/components/basebutton.hpp
    ui/components/serverbutton.hpp
    ui/components/localbutton.hpp
    ui/components/pagebutton.hpp
    )

set (GUMPMENU_CPP
    ui/gumpfactory.cpp
    ui/gumpmenu.cpp
    ui/gumpelement.cpp
    ui/ingameview.cpp

    ui/components/basebutton.cpp
    ui/components/serverbutton.cpp
    ui/components/localbutton.cpp
    ui/components/pagebutton.cpp
    )

set (GUMPS_HPP
    ui/gumps/shardselection.hpp
    )

set (GUMPS_CPP
    ui/gumps/shardselection.cpp
    )

SET(UI_FILES ${UI_HPP} ${UI_CPP} ${GUMPMENU_HPP} ${GUMPMENU_CPP} ${GUMPS_HPP} ${GUMPS_CPP} PARENT_SCOPE)

