cmake_minimum_required(VERSION 3.10)
project(destroid)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)

find_package(SDL2 REQUIRED)
add_subdirectory(thirdparty)

add_executable(destroid
    src/base/application.cpp
    src/base/application.hpp
    src/base/game.cpp
    src/base/game.hpp
    src/base/window.cpp
    src/base/window.hpp
    src/component/player.hpp
    src/component/drawable.hpp
    src/component/transform.hpp
    src/component/velocity.hpp
    src/component/collider.hpp
    src/component/asteroid.hpp
    src/graphics/renderer.cpp
    src/graphics/renderer.hpp
    src/graphics/shader.cpp
    src/graphics/shader.hpp
    src/graphics/texture.cpp
    src/graphics/texture.hpp
    src/math/rectangle.hpp
    src/system/collision.cpp
    src/system/collision.hpp
    src/system/drawing.cpp
    src/system/drawing.hpp
    src/system/input.cpp
    src/system/input.hpp
    src/system/physics.cpp
    src/system/physics.hpp
    src/main.cpp)

target_include_directories(destroid PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_compile_options(destroid PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Wshadow
        -Wcast-align
        -Wdouble-promotion
        -Wold-style-cast)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    # Can't add /W4 to 'target_compile_options' directly, because older versions of CMake add /W3
    # implicitly (for some reason) to CMAKE_CXX_FLAGS. Since MSVC will complain if there are
    # multiple warning levels enabled, /W4 has to be added by replacing the existing /W3 flag.
    string(REGEX REPLACE "/W3" "/W4" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})

    target_compile_options(destroid PRIVATE
        /permissive-
        /Zc:__cplusplus
        /EHsc
        /MP)
endif()

# Force the use of standard C++17
set_target_properties(destroid PROPERTIES
    CXX_STANDARD 17
    CXX_EXTENSIONS OFF
    CXX_STANDARD_REQUIRED ON)

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
