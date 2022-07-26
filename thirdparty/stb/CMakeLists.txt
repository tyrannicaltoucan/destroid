add_library(stb_image STATIC
    src/stb_image.c
    include/stb_image.h
)

target_include_directories(stb_image PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
add_library(stb::image ALIAS stb_image)
