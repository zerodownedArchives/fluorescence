
add_subdirectory(data)
add_subdirectory(misc)
add_subdirectory(ui)
add_subdirectory(world)
add_subdirectory(net)

set(CLIENT_HPP
    client.hpp
    )

set(CLIENT_CPP
    client.cpp
    main.cpp
    )

add_executable(client ${DATA_FILES} ${MISC_FILES} ${NET_FILES} ${UI_FILES} ${WORLD_FILES}  ${CLIENT_HPP} ${CLIENT_CPP} )
target_link_libraries(client ${UOME_LIBRARIES})
