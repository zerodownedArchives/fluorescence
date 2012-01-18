
cmake_minimum_required (VERSION 2.8)
project (fluorescence)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

IF (NOT "${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
  set(CMAKE_BUILD_TYPE Release)
ENDIF (NOT "${CMAKE_BUILD_TYPE}" STREQUAL "Debug")


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ")


set(CMAKE_CXX_FLAGS "-Wall -Werror ${CMAKE_CXX_FLAGS} -msse2 -D_REENTRANT --std=c++0x -DBOOST_FILESYSTEM_VERSION=2")
set(CMAKE_CXX_FLAGS_DEBUG "-ggdb -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(ClanLib_VERSION_LIBNAME "23")
set(ClanLib_VERSION_DIRNAME "2.3")
find_package(ClanLib REQUIRED COMPONENTS App Core Display GL GUI)

find_package(X11 REQUIRED COMPONENTS cursor)

EXEC_PROGRAM(icu-config
    ARGS "--ldflags" "--ldflags-icuio"
    OUTPUT_VARIABLE ICU_LIBRARIES)

include_directories(${ClanLib_INCLUDE_DIRS})

set(Boost_USE_MULTITHREADED ON)
find_package(Boost 1.42.0 REQUIRED COMPONENTS filesystem system thread program_options)

set(FLUO_LIBRARIES ${ClanLib_LIBRARIES} ${Boost_LIBRARIES} ${ICU_LIBRARIES} -lXcursor)


set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}")

include_directories("src/fluorescence")

add_subdirectory(src)
