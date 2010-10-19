
add_subdirectory(data)
add_subdirectory(ui)

set(CLIENT_HPP
    client.hpp
    logger.hpp
    string.hpp
    config.hpp
    )

set(CLIENT_CPP
    client.cpp
    )

add_executable(client ${CLIENT_HPP} ${CLIENT_CPP})
target_link_libraries(client ${UOME_LIBRARIES})
