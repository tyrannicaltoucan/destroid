# FindSDL2 - CMake find module for SDL2
# --------------------------------------------------------------
# This module defines the following:
#
# -- Public
# SDL2::SDL2     - SDL2 core target
# SDL2::SDL2main - SDL2 main target
# SDL2_FOUND     - Returns true if SDL2 has been located
# SDL2MAIN_FOUND - Returns true if SDL2main has been located
#
# -- Internal
# SDL2_LIBRARY_RELEASE     - SDL2 release library location
# SDL2_LIBRARY_DEBUG       - SDL2 debug library location
# SDL2MAIN_LIBRARY_RELEASE - SDL2 release library location
# SDL2MAIN_LIBRARY_DEBUG   - SDL2 debug library location
# SDL2_INCLUDE_DIR         - SDL2 include directory
# --------------------------------------------------------------

if(CMAKE_C_COMPILER_ID MATCHES "MSVC")
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(_SDL2_LIB_PATH_SUFFIX lib/x64)
    else()
        set(_SDL2_LIB_PATH_SUFFIX lib/x86)
    endif()
endif()

find_library(SDL2_LIBRARY_RELEASE
    NAMES SDL2
    PATH_SUFFIXES lib ${_SDL2_LIB_PATH_SUFFIX})

find_library(SDL2_LIBRARY_DEBUG
    NAMES SDL2d
    PATH_SUFFIXES lib ${_SDL2_LIB_PATH_SUFFIX})

find_library(SDL2MAIN_LIBRARY_RELEASE
    NAMES SDL2main
    PATH_SUFFIXES lib ${_SDL2_LIB_PATH_SUFFIX})

find_library(SDL2MAIN_LIBRARY_DEBUG
    NAMES SDL2maind
    PATH_SUFFIXES lib ${_SDL2_LIB_PATH_SUFFIX})

find_path(SDL2_INCLUDE_DIR
    # Check for a file that only exists with SDL2 in case SDL1 is also installed.
    NAMES SDL_assert.h
    PATH_SUFFIXES SDL2)

include(SelectLibraryConfigurations)
select_library_configurations(SDL2)
select_library_configurations(SDL2main)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(SDL2 DEFAULT_MSG SDL2_LIBRARY SDL2_INCLUDE_DIR)

if(NOT TARGET SDL2::SDL2)
    add_library(SDL2::SDL2 UNKNOWN IMPORTED)

    if(SDL2_LIBRARY_RELEASE)
        set_property(TARGET SDL2::SDL2 APPEND PROPERTY
            IMPORTED_CONFIGURATIONS RELEASE)

        set_property(TARGET SDL2::SDL2 PROPERTY
            IMPORTED_LOCATION_RELEASE ${SDL2_LIBRARY_RELEASE})
    endif()

    if(SDL2_LIBRARY_DEBUG)
        set_property(TARGET SDL2::SDL2 APPEND PROPERTY
            IMPORTED_CONFIGURATIONS DEBUG)

        set_property(TARGET SDL2::SDL2 PROPERTY
            IMPORTED_LOCATION_DEBUG ${SDL2_LIBRARY_DEBUG})
    endif()

    set_property(TARGET SDL2::SDL2 PROPERTY
        INTERFACE_INCLUDE_DIRECTORIES ${SDL2_INCLUDE_DIR})
endif()

if(NOT TARGET SDL2::SDL2main)
    add_library(SDL2::SDL2main STATIC IMPORTED)

    if(SDL2MAIN_LIBRARY_RELEASE)
        set_property(TARGET SDL2::SDL2main APPEND PROPERTY
            IMPORTED_CONFIGURATIONS RELEASE)

        set_property(TARGET SDL2::SDL2main PROPERTY
            IMPORTED_LOCATION_RELEASE ${SDL2MAIN_LIBRARY_RELEASE})
    endif()

    if(SDL2MAIN_LIBRARY_DEBUG)
        set_property(TARGET SDL2::SDL2main APPEND PROPERTY
            IMPORTED_CONFIGURATIONS DEBUG)

        set_property(TARGET SDL2::SDL2main PROPERTY
            IMPORTED_LOCATION_DEBUG ${SDL2MAIN_LIBRARY_DEBUG})
    endif()

    set_property(TARGET SDL2::SDL2main PROPERTY
        # Linking SDL2::SDL2main implicitly includes SDL2::SDL2 as well.
        INTERFACE_LINK_LIBRARIES SDL2::SDL2)
endif()

# Hide internal variables from CMake GUI.
mark_as_advanced(SDL2_INCLUDE_DIR
    SDL2_LIBRARY_RELEASE
    SDL2_LIBRARY_DEBUG
    SDL2MAIN_LIBRARY_RELEASE
    SDL2MAIN_LIBRARY_DEBUG)
