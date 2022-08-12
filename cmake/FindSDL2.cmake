# FindSDL2 CMake module
#
# The following targets are exported:
#   * SDL2::SDL2 -- Primary SDL library
#   * SDL2::SDL2main -- Platform entry point helper
#
# An environment variable named SDL2DIR can be set to specify a custom search
# location.

if(CMAKE_C_COMPILER_ID MATCHES "MSVC")
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(_SDL2_LIBRARY_PATH_SUFFIX lib/x64)
    else()
        set(_SDL2_LIBRARY_PATH_SUFFIX lib/x86)
    endif()
endif()

find_library(SDL2_LIBRARY_RELEASE
    HINTS "$ENV{SDL2DIR}"
    NAMES SDL2 SDL2-2.0
    PATH_SUFFIXES "${_SDL2_LIBRARY_PATH_SUFFIX}"
)

find_library(SDL2_LIBRARY_DEBUG
    HINTS "$ENV{SDL2DIR}"
    NAMES SDL2d
    PATH_SUFFIXES "${_SDL2_LIBRARY_PATH_SUFFIX}"
)

find_library(SDL2MAIN_LIBRARY
    HINTS "$ENV{SDL2DIR}"
    NAMES SDL2main
    PATH_SUFFIXES lib "${_SDL2_LIBRARY_PATH_SUFFIX}"
)

include(SelectLibraryConfigurations)
select_library_configurations(SDL2)

find_path(SDL2_INCLUDE_DIR
    HINTS "$ENV{SDL2DIR}"
    NAMES SDL_assert.h
    PATH_SUFFIXES SDL2 include/SDL2 include
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(SDL2 DEFAULT_MSG SDL2_LIBRARY SDL2_INCLUDE_DIR)

if(SDL2MAIN_LIBRARY)
    set(FPHSA_NAME_MISMATCHED ON)
    find_package_handle_standard_args(SDL2main DEFAULT_MSG SDL2MAIN_LIBRARY SDL2_INCLUDE_DIR)
endif()

if(NOT TARGET SDL2::SDL2)
    add_library(SDL2::SDL2 INTERFACE IMPORTED)
    target_include_directories(SDL2::SDL2 INTERFACE "${SDL2_INCLUDE_DIR}")

    if(SDL2_LIBRARY_RELEASE)
        target_link_libraries(SDL2::SDL2 INTERFACE "${SDL2_LIBRARY_RELEASE}")
    elseif(SDL2_LIBRARY_DEBUG)
        target_link_libraries(SDL2::SDL2 INTERFACE "${SDL2_LIBRARY_DEBUG}")
    endif()
endif()

if(NOT TARGET SDL2::SDL2main)
    add_library(SDL2::SDL2main INTERFACE IMPORTED)

    if(NOT SDL2MAIN_LIBRARY)
        target_link_libraries(SDL2::SDL2main INTERFACE SDL2::SDL2)
    else()
        target_link_libraries(SDL2::SDL2main INTERFACE "${SDL2MAIN_LIBRARY}" SDL2::SDL2)
    endif()
endif()

mark_as_advanced(SDL2_INCLUDE_DIR)
