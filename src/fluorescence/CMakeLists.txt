
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse2 -D_REENTRANT -DBOOST_FILESYSTEM_VERSION=2")

set(ClanLib_VERSION_LIBNAME "23")
set(ClanLib_VERSION_DIRNAME "2.3")
find_package(ClanLib REQUIRED COMPONENTS App Core Display GL GUI)

find_package(X11 REQUIRED COMPONENTS cursor)

EXEC_PROGRAM(icu-config
    ARGS "--ldflags" "--ldflags-icuio"
    OUTPUT_VARIABLE ICU_LIBRARIES)

include_directories(${ClanLib_INCLUDE_DIRS})

set(Boost_USE_MULTITHREADED ON)
find_package(Boost 1.46.0 REQUIRED COMPONENTS filesystem system thread program_options python)

find_package(PythonLibs 2.7 REQUIRED)

set(FLUO_LIBRARIES ${ClanLib_LIBRARIES} ${Boost_LIBRARIES} ${ICU_LIBRARIES} -lXcursor -lcurl -lfmodex ${PYTHON_LIBRARIES})

include_directories(".")
include_directories(${PYTHON_INCLUDE_DIRS})


add_subdirectory(data)
add_subdirectory(misc)
add_subdirectory(ui)
add_subdirectory(world)
add_subdirectory(net)

set(CLIENT_HPP
    client.hpp
    typedefs.hpp
    platform.hpp
    )

set(CLIENT_CPP
    client.cpp
    main.cpp
    platform.cpp
    )

add_executable(fluorescence ${DATA_FILES} ${MISC_FILES} ${NET_FILES} ${UI_FILES} ${WORLD_FILES}  ${CLIENT_HPP} ${CLIENT_CPP} )
target_link_libraries(fluorescence ${FLUO_LIBRARIES})
