
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
    ui/gumpmenus.hpp
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
    ui/gumpmenus.cpp
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
    ui/components/scrollarea.hpp
    ui/components/lineedit.hpp
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
    ui/components/scrollarea.cpp
    ui/components/lineedit.cpp
    )

SET(UI_FILES ${UI_HPP} ${UI_CPP} ${GUMPMENU_HPP} ${GUMPMENU_CPP} PARENT_SCOPE)

