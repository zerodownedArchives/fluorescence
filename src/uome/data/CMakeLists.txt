
set (DATA_HPP
    fullfileloader.hpp
    ondemandreadable.hpp
    ondemandfileloader.hpp
    manager.hpp
    weakptrcache.hpp
    indexedondemandfileloader.hpp
    fixedsizeondemandfileloader.hpp
    util.hpp
    )

set (DATA_CPP
    fullfileloader.cpp
    manager.cpp
    )

set(DATA_LOADERS_HPP
    indexloader.hpp
    artloader.hpp
    tiledataloader.hpp
    huesloader.hpp
    gumpartloader.hpp
    maploader.hpp
    )

set(DATA_LOADERS_CPP
    indexloader.cpp
    artloader.cpp
    tiledataloader.cpp
    huesloader.cpp
    gumpartloader.cpp
    maploader.cpp
    )

add_library(uomedata STATIC
    ${DATA_HPP} ${DATA_CPP}
    ${DATA_LOADERS_HPP} ${DATA_LOADERS_CPP})

add_subdirectory(test)

