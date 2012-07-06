
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

set(ClanLib_VERSION_LIBNAME "23")
set(ClanLib_VERSION_DIRNAME "2.3")
find_package(ClanLib REQUIRED COMPONENTS App Core Display GUI SWRender)

include_directories(${ClanLib_INCLUDE_DIRS})

include_directories("/usr/include/subversion-1" "/usr/include/apr-1.0")

set(Boost_USE_MULTITHREADED ON)
find_package(Boost 1.46.0 REQUIRED COMPONENTS system thread)

set(UPDATER_LIBRARIES ${ClanLib_LIBRARIES} ${Boost_LIBRARIES} -lpthread -lsvncpp)


include_directories(".")

set(UPDATER_HPP
    updater.hpp
    platform.hpp
    svn.hpp

    misc/log.hpp
    ui.hpp
    )

set(UPDATER_CPP
    main.cpp
    updater.cpp
    platform.cpp
    svn.cpp

    misc/log.cpp
    ui.cpp
    )

add_executable(updater ${UPDATER_CPP} ${UPDATER_HPP})
target_link_libraries(updater ${UPDATER_LIBRARIES})
