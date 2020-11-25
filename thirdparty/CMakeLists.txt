add_library(glm INTERFACE)
target_include_directories(glm SYSTEM INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/glm)

add_library(entt INTERFACE)
target_include_directories(glm SYSTEM INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/entt/src)

add_subdirectory(glad)
add_subdirectory(stb)
