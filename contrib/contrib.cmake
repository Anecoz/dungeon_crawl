if (WIN32)
  # SFML
  set(SFML_DIR ${CMAKE_SOURCE_DIR}/contrib/win/SFML-2.5.1/lib/cmake/SFML)
  find_package(SFML COMPONENTS graphics window system)

  if (NOT SFML_FOUND)
    message(FATAL "SFML Not found!")
  endif()

  # yojimbo
  #set(YOJIMBO_VERSION "v1.2.1")
  #add_library(yojimbo STATIC IMPORTED REQUIRED)
  #set_target_properties(yojimbo PROPERTIES IMPORTED_LOCATION_DEBUG ${CMAKE_CURRENT_LIST_DIR}/win/yojimbo/${YOJIMBO_VERSION}/lib/Debug/yojimbo.lib)
  #set_target_properties(yojimbo PROPERTIES IMPORTED_LOCATION_RELEASE ${CMAKE_CURRENT_LIST_DIR}/win/yojimbo/${YOJIMBO_VERSION}/lib/Release/yojimbo.lib)
  #target_include_directories(yojimbo INTERFACE ${CMAKE_CURRENT_LIST_DIR}/win/yojimbo/${YOJIMBO_VERSION}/include)

else()
  message(FATAL "Contribs only exist for Win32, aborting!")
endif()

# Shared on all platforms
set(glm_DIR ${CMAKE_SOURCE_DIR}/contrib/glm-0.9.9.8/glm/cmake/glm)
find_package(glm)

if (NOT glm_FOUND)
  message(FATAL "glm not found!")
endif()