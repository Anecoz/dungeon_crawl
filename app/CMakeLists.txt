file(GLOB_RECURSE src
  ${CMAKE_CURRENT_SOURCE_DIR}/*.h
  ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_definitions(-DRESOURCE_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")

add_executable(dc ${src})

target_link_libraries(dc sfml-graphics sfml-window sfml-system glm::glm)

# Get rid of 'missing pdb' type of warnings when linking.
if (WIN32)
  set_target_properties(dc PROPERTIES LINK_FLAGS "/ignore:4099 /ignore:4098")
endif()