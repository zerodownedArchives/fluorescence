
add_subdirectory(data)
add_subdirectory(misc)
add_subdirectory(ui)
add_subdirectory(world)

set(CLIENT_HPP
    client.hpp
    )

set(CLIENT_CPP
    client.cpp
    main.cpp
    )

add_executable(client ${CLIENT_HPP} ${CLIENT_CPP})
target_link_libraries(client ${UOME_LIBRARIES})
