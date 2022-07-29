cmake_minimum_required(VERSION 3.10)
project(destroid)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(SDL2 REQUIRED)
add_subdirectory(thirdparty)

add_executable(destroid
    src/entity/components/collider.hpp
    src/entity/components/drag.hpp
    src/entity/components/drawable.hpp
    src/entity/components/lifetime.hpp
    src/entity/components/momentum.hpp
    src/entity/components/spawner.hpp
    src/entity/components/tag.hpp
    src/entity/components/thrust.hpp
    src/entity/components/transform.hpp
    src/entity/components/weapon.hpp
    src/entity/factory.cpp
    src/entity/factory.hpp
    src/graphics/renderer.cpp
    src/graphics/renderer.hpp
    src/graphics/shader.cpp
    src/graphics/shader.hpp
    src/graphics/texture.cpp
    src/graphics/texture.hpp
    src/math/circle.hpp
    src/math/random.hpp
    src/math/rectangle.hpp
    src/systems/collision.cpp
    src/systems/collision.hpp
    src/systems/despawn.cpp
    src/systems/despawn.hpp
    src/systems/drawing.cpp
    src/systems/drawing.hpp
    src/systems/expiry.cpp
    src/systems/expiry.hpp
    src/systems/input.cpp
    src/systems/input.hpp
    src/systems/movement.cpp
    src/systems/movement.hpp
    src/systems/spawning.cpp
    src/systems/spawning.hpp
    src/systems/weapon.cpp
    src/systems/weapon.hpp
    src/game_scene.cpp
    src/game_scene.hpp
    src/game.cpp
    src/game.hpp
    src/main.cpp
)

target_include_directories(destroid PRIVATE "${PROJECT_SOURCE_DIR}/src")

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    target_compile_options(destroid PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Wsign-compare
        -Wold-style-cast
    )
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    target_compile_options(destroid PRIVATE
        /permissive-
        /Zc:__cplusplus
        /EHsc
        /MP
    )
endif()

target_link_libraries(destroid PRIVATE
    EnTT::EnTT
    glad::glad
    glm::glm
    stb::image
    SDL2::SDL2main
)
