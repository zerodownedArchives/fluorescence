
add_executable(test_indexloader testindexloader.cpp)
target_link_libraries(test_indexloader ${UOME_LIBRARIES})

add_executable(test_ondemandloader testondemandloader.cpp)
target_link_libraries(test_ondemandloader ${UOME_LIBRARIES})

add_executable(test_weakptrcache testweakptrcache.cpp)
target_link_libraries(test_weakptrcache ${UOME_LIBRARIES})
