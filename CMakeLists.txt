
cmake_minimum_required (VERSION 2.8)
project (uome)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

IF (NOT "${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
  set(CMAKE_BUILD_TYPE Release)
ENDIF (NOT "${CMAKE_BUILD_TYPE}" STREQUAL "Debug")


set(Boost_USE_MULTITHREADED ON)
find_package(Boost 1.40.0 REQUIRED COMPONENTS filesystem thread program_options)

set(ClanLib_VERSION_LIBNAME "22")
set(ClanList_VERSION_DIRNAME "2.2")
find_package(ClanLib REQUIRED)

EXEC_PROGRAM(icu-config
        ARGS "--ldflags" "--ldflags-icuio"
        OUTPUT_VARIABLE ICU_LIBRARIES)

#message(STATUS "boost libs: ${Boost_LIBRARIES}")
#message(STATUS "cl libs: ${ClanLib_LIBRARIES}")
#message(STATUS "icu libs: ${ICU_LIBRARIES}")

set(UOME_LIBRARIES uomedata uomeui uomeworld uomemisc ${ClanLib_LIBRARIES} ${Boost_LIBRARIES} ${ICU_LIBRARIES})

set(CMAKE_CXX_FLAGS "-Wall -Werror ${CMAKE_CXX_FLAGS} -msse2 -D_REENTRANT")
set(CMAKE_CXX_FLAGS_DEBUG "-g -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/bin/")
set(LIBRARY_OUTPUT_PATH "${EXECUTABLE_OUTPUT_PATH}")

include_directories("src/uome" ${ClanLib_INCLUDE_DIRS})

add_subdirectory(src)
