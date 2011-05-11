
set (DATA_HPP
    data/fullfileloader.hpp
    data/ondemandreadable.hpp
    data/ondemandfileloader.hpp
    data/manager.hpp
    data/weakptrcache.hpp
    data/indexedondemandfileloader.hpp
    data/fixedsizeondemandfileloader.hpp
    data/util.hpp
    )

set (DATA_CPP
    data/fullfileloader.cpp
    data/manager.cpp
    )

set(DATA_LOADERS_HPP
    data/indexloader.hpp
    data/artloader.hpp
    data/tiledataloader.hpp
    data/huesloader.hpp
    data/gumpartloader.hpp
    data/maploader.hpp
    data/staticsloader.hpp
    data/maptexloader.hpp
    data/animdataloader.hpp
    data/animloader.hpp

    data/mobtypesloader.hpp
    )

set(DATA_LOADERS_CPP
    data/indexloader.cpp
    data/artloader.cpp
    data/tiledataloader.cpp
    data/huesloader.cpp
    data/gumpartloader.cpp
    data/maploader.cpp
    data/staticsloader.cpp
    data/maptexloader.cpp
    data/animdataloader.cpp
    data/animloader.cpp

    data/mobtypesloader.cpp
    )

set (DATA_FILES
    ${DATA_HPP} ${DATA_CPP}
    ${DATA_LOADERS_HPP} ${DATA_LOADERS_CPP} PARENT_SCOPE)

#add_subdirectory(test)

