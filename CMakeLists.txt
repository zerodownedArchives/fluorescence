
cmake_minimum_required (VERSION 2.8)
project (fluorescence)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

IF (NOT "${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
  set(CMAKE_BUILD_TYPE Release)
ENDIF (NOT "${CMAKE_BUILD_TYPE}" STREQUAL "Debug")


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror --std=c++0x")
set(CMAKE_CXX_FLAGS_DEBUG "-ggdb -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")


set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}")


add_subdirectory(src)
