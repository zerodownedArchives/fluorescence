
set (DATA_HPP
    fullfileloader.hpp
    ondemandfileloader.hpp
    manager.hpp
    weakptrcache.hpp
    indexedondemandfileloader.hpp
    fixedsizeondemandfileloader.hpp
    util.hpp
    )

set (DATA_CPP
    manager.cpp
    )

set(DATA_STRUCTURES_HPP
    indexblock.hpp
    mapblock.hpp
    tiledata.hpp
    )

set(DATA_STRUCTURES_CPP
    tiledata.cpp
    )
    
set(DATA_LOADERS_HPP
    artloader.hpp
    )
    
set(DATA_LOADERS_CPP
    artloader.cpp
    )

add_library(uomedata STATIC 
    ${DATA_HPP} ${DATA_CPP} 
    ${DATA_STRUCTURES_HPP} ${DATA_STRUCTURES_CPP}
    ${DATA_LOADERS_HPP} ${DATA_LOADERS_CPP})

add_subdirectory(test)

