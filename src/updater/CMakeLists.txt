
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-declarations")

set(ClanLib_VERSION_LIBNAME "23")
set(ClanLib_VERSION_DIRNAME "2.3")
find_package(ClanLib REQUIRED COMPONENTS App Core Display GUI SWRender)

include_directories(${ClanLib_INCLUDE_DIRS})

include_directories("/usr/include/subversion-1" "/usr/include/apr-1.0")

set(Boost_USE_MULTITHREADED ON)
find_package(Boost 1.46.0 REQUIRED COMPONENTS system thread)

set(UPDATER_LIBRARIES ${ClanLib_LIBRARIES} ${Boost_LIBRARIES} -lpthread -lapr-1 -lsvn_client-1)


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

set (SVNCPP_FILES
    svncpp/annotate_line.hpp
    svncpp/apr.cpp
    svncpp/apr.hpp
    svncpp/client_annotate.cpp
    svncpp/client_cat.cpp
    svncpp/client.cpp
    svncpp/client_diff.cpp
    svncpp/client.hpp
    svncpp/client_ls.cpp
    svncpp/client_modify.cpp
    svncpp/client_property.cpp
    svncpp/client_status.cpp
    svncpp/context.cpp
    svncpp/context.hpp
    svncpp/context_listener.hpp
    svncpp/datetime.cpp
    svncpp/datetime.hpp
    svncpp/dirent.cpp
    svncpp/dirent.hpp
    svncpp/entry.cpp
    svncpp/entry.hpp
    svncpp/exception.cpp
    svncpp/exception.hpp
    svncpp/info.cpp
    svncpp/info.hpp
    svncpp/log_entry.cpp
    svncpp/log_entry.hpp
    svncpp/map_wrapper.hpp
    svncpp/m_check.hpp
    svncpp/path.cpp
    svncpp/path.hpp
    svncpp/pool.cpp
    svncpp/pool.hpp
    svncpp/property.cpp
    svncpp/property.hpp
    svncpp/revision.cpp
    svncpp/revision.hpp
    svncpp/status.cpp
    svncpp/status.hpp
    svncpp/status_selection.cpp
    svncpp/status_selection.hpp
    svncpp/string_wrapper.hpp
    svncpp/targets.cpp
    svncpp/targets.hpp
    svncpp/url.cpp
    svncpp/url.hpp
    svncpp/utility_wrapper.hpp
    svncpp/vector_wrapper.hpp
    svncpp/version.hpp
    svncpp/wc.cpp
    svncpp/wc.hpp
    )

add_executable(updater ${UPDATER_CPP} ${UPDATER_HPP} ${SVNCPP_FILES})
target_link_libraries(updater ${UPDATER_LIBRARIES})
