cmake_minimum_required(VERSION 3.10)
project(destroid)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)

find_package(SDL2 REQUIRED)
add_subdirectory(thirdparty)

add_executable(destroid
    src/base/circle.hpp
    src/base/rectangle.hpp
    src/component/drag.hpp
    src/component/drawable.hpp
    src/component/momentum.hpp
    src/component/transform.hpp
    src/component/thrust.hpp
    src/component/momentum.hpp
    src/component/lifetime.hpp
    src/component/weapon.hpp
    src/entity/factory.cpp
    src/entity/factory.hpp
    src/entity/tags.hpp
    src/game/game.cpp
    src/game/game.hpp
    src/graphics/renderer.cpp
    src/graphics/renderer.hpp
    src/graphics/shader.cpp
    src/graphics/shader.hpp
    src/graphics/texture.cpp
    src/graphics/texture.hpp
    src/system/collision.cpp
    src/system/collision.hpp
    src/system/despawn.cpp
    src/system/despawn.hpp
    src/system/drawing.cpp
    src/system/drawing.hpp
    src/system/expiry.cpp
    src/system/expiry.hpp
    src/system/input.cpp
    src/system/input.hpp
    src/system/movement.cpp
    src/system/movement.hpp
    src/system/weapon.cpp
    src/system/weapon.hpp
    src/main.cpp)

target_include_directories(destroid PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_compile_options(destroid PRIVATE
    $<$<CXX_COMPILER_ID:Clang,GNU>:
        -Wall
        -Wextra
        -Wpedantic
        -Wdouble-promotion
        -Wold-style-cast>
    $<$<CXX_COMPILER_ID:MSVC>:
        /permissive-
        /Zc:__cplusplus
        /W4
        /MP>)

target_compile_features(destroid PRIVATE cxx_std_17)

target_compile_definitions(destroid PRIVATE
    STBI_ONLY_PNG
    GLM_FORCE_CXX17)

target_link_libraries(destroid PRIVATE
    entt
    glad
    glm
    stb_image
    SDL2::SDL2main)

add_custom_command(TARGET destroid POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${PROJECT_SOURCE_DIR}/assets $<TARGET_FILE_DIR:destroid>/assets
    COMMENT "Copying assets to ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
