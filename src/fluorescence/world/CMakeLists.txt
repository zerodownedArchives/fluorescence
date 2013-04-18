
set (WORLD_HPP
    world/manager.hpp
    world/ingameobject.hpp
    world/map.hpp
    world/statics.hpp
    world/sector.hpp
    world/sectormanager.hpp
    world/lightmanager.hpp
    world/serverobject.hpp
    world/mobile.hpp
    world/dynamicitem.hpp
    world/overheadmessage.hpp
    world/smoothmovement.hpp
    world/smoothmovementmanager.hpp
    world/particleeffect.hpp
    world/playerwalkmanager.hpp
    world/effect.hpp
    world/osieffect.hpp
    world/syslog.hpp
    world/buffinfo.hpp
    )

set (WORLD_CPP
    world/manager.cpp
    world/ingameobject.cpp
    world/map.cpp
    world/statics.cpp
    world/sector.cpp
    world/sectormanager.cpp
    world/lightmanager.cpp
    world/serverobject.cpp
    world/mobile.cpp
    world/dynamicitem.cpp
    world/overheadmessage.cpp
    world/smoothmovement.cpp
    world/smoothmovementmanager.cpp
    world/particleeffect.cpp
    world/playerwalkmanager.cpp
    world/effect.cpp
    world/osieffect.cpp
    world/syslog.cpp
    )

set(WORLD_FILES ${WORLD_HPP} ${WORLD_CPP} PARENT_SCOPE)
