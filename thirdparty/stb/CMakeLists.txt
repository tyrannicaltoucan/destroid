add_library(stb_image STATIC
    "stb_image.h"
    "stb_image.c"
)

target_include_directories(stb_image PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
