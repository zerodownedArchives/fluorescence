
cmake_minimum_required (VERSION 2.8)
project (fluorescence)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

IF (NOT "${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
  set(CMAKE_BUILD_TYPE Release)
ENDIF (NOT "${CMAKE_BUILD_TYPE}" STREQUAL "Debug")



# platform specific stuff
IF (CMAKE_COMPILER_IS_GNUCXX)
    MESSAGE(STATUS "Compiling with gnu compiler")

    set(CMAKE_CXX_FLAGS "-Wall -Werror ${CMAKE_CXX_FLAGS} -msse2 -D_REENTRANT --std=c++0x")
    set(CMAKE_CXX_FLAGS_DEBUG "-g -DDEBUG")
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
	find_package(Boost 1.40.0 REQUIRED COMPONENTS filesystem thread program_options)

	set(FLUO_LIBRARIES ${ClanLib_LIBRARIES} ${Boost_LIBRARIES} ${ICU_LIBRARIES} -lXcursor)

ENDIF(CMAKE_COMPILER_IS_GNUCXX)

IF (WIN32)
    MESSAGE(STATUS "Compiling for windows")

    include_directories(${CMAKE_SOURCE_DIR}/winbuild/include)
    link_directories(${CMAKE_SOURCE_DIR}/winbuild/lib32)

    set (MYLIBDIR ${CMAKE_SOURCE_DIR}/winbuild/lib32/)

    #set(BOOST_LIBRARYDIR ${MYLIBDIR})
    #set(BOOST_INCLUDEDIR ${CMAKE_SOURCE_DIR}/winbuild/include)

	#MESSAGE(STATUS ${CMAKE_SOURCE_DIR})

    set(FLUO_LIBRARIES		${MYLIBDIR}clanApp-static-mt.lib 
							${MYLIBDIR}clanCore-static-mt.lib 
							${MYLIBDIR}clanDisplay-static-mt.lib 
							${MYLIBDIR}clanGL-static-mt.lib 
							${MYLIBDIR}clanGUI-static-mt.lib
							
							 ${MYLIBDIR}icuin.lib
							 ${MYLIBDIR}icuuc.lib 
							 ${MYLIBDIR}icudt.lib 
							 ${MYLIBDIR}icuio.lib

							 ${MYLIBDIR}libboost_filesystem-vc100-mt-s-1_47.lib
							 ${MYLIBDIR}libboost_program_options-vc100-mt-s-1_47.lib
							 ${MYLIBDIR}libboost_thread-vc100-mt-s-1_47.lib
	)

	MESSAGE(STATUS ${ClanLib_LIBRARIES})

    set(ICU_LIBRARIES ${MYLIBDIR}icuin.lib ${MYLIBDIR}icuuc.lib ${MYLIBDIR}icudt.lib ${MYLIBDIR}icuio.lib)

ENDIF(WIN32)

set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}")

include_directories("src/fluorescence")

add_subdirectory(src)
