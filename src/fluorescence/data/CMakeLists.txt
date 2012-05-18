
set (DATA_HPP
    data/fullfileloader.hpp
    data/ondemandreadable.hpp
    data/ondemandfileloader.hpp
    data/manager.hpp
    data/weakptrcache.hpp
    data/indexedondemandfileloader.hpp
    data/fixedsizeondemandfileloader.hpp
    data/util.hpp
    data/ondemandurlloader.hpp
    )

set (DATA_CPP
    data/fullfileloader.cpp
    data/manager.cpp
    data/util.cpp
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
    data/unifontloader.hpp
    data/clilocloader.hpp
    data/soundloader.hpp
    data/sound.hpp
    
    data/httploader.hpp
    data/filepathloader.hpp

    data/mobtypesloader.hpp
    data/deffileloader.hpp
    data/equipconvdefloader.hpp
    data/spellbooks.hpp
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
    data/unifontloader.cpp
    data/clilocloader.cpp
    data/soundloader.cpp
    data/sound.cpp
    
    data/httploader.cpp
    data/filepathloader.cpp

    data/mobtypesloader.cpp
    data/equipconvdefloader.cpp
    data/spellbooks.cpp
    )

set (DATA_FILES
    ${DATA_HPP} ${DATA_CPP}
    ${DATA_LOADERS_HPP} ${DATA_LOADERS_CPP} PARENT_SCOPE)
