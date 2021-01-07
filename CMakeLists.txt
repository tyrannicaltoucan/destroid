cmake_minimum_required(VERSION 3.10)
project(destroid)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)

find_package(SDL2 REQUIRED)
add_subdirectory(thirdparty)

add_executable(destroid
    src/entity/component/ship.hpp
    src/entity/component/drawable.hpp
    src/entity/component/transform.hpp
    src/entity/component/body.hpp
    src/entity/component/collider.hpp
    src/entity/component/lifetime.hpp
    src/entity/component/weapon.hpp
    src/entity/system/collision.cpp
    src/entity/system/collision.hpp
    src/entity/system/despawn.cpp
    src/entity/system/despawn.hpp
    src/entity/system/drawing.cpp
    src/entity/system/drawing.hpp
    src/entity/system/input.cpp
    src/entity/system/input.hpp
    src/entity/system/firing.cpp
    src/entity/system/firing.hpp
    src/entity/system/physics.cpp
    src/entity/system/physics.hpp
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
    src/math/rectangle.hpp
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
