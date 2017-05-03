# - Find the PortAudio library
#
# Usage:
#   find_package(PORTAUDIO [REQUIRED] [QUIET])
#
# It sets the following variables:
#   PORTAUDIO_FOUND               ... true if portaudio is found on the system
#   PORTAUDIO_LIBRARIES           ... full path to portaudio library
#   PORTAUDIO_INCLUDE_DIRS        ... portaudio include directory
#
# The following variables will be checked by the function
#   PORTAUDIO_ROOT               ... if set, the libraries are exclusively searched
#                               under this path

# Check if we can use PkgConfig
find_package(PkgConfig QUIET)
if(PKG_CONFIG_FOUND)
    pkg_check_modules(PKGCONFIG_PORTAUDIO "portaudio-2.0")
endif()

# portaudio source root specified
if(PORTAUDIO_ROOT)
    message(STATUS "PortAudio root: ${PORTAUDIO_ROOT}")

    find_path(PORTAUDIO_INCLUDE_DIR
        NAMES portaudio.h
        PATHS ${PORTAUDIO_ROOT}
        PATH_SUFFIXES "include"
        NO_DEFAULT_PATH)

    find_library(PORTAUDIO_LIBRARY
        NAMES portaudio libportaudio
        PATHS ${PORTAUDIO_ROOT}
        PATH_SUFFIXES "lib" "lib64"
        NO_DEFAULT_PATH)

else()

    find_path(PORTAUDIO_INCLUDE_DIR
        NAMES portaudio.h
        HINTS ${PKGCONFIG_PORTAUDIO_INCLUDEDIR} ${PKGCONFIG_PORTAUDIO_INCLUDE_DIRS}
        PATHS
          /usr/include
          /usr/local/include
          /opt/local/include
          /sw/include
          /usr/local/mingw/i686-w64-mingw32/include
          C:/MinGW/msys/1.0/usr/include
          C:/MinGW/msys/1.0/usr/local/include)

    find_library(PORTAUDIO_LIBRARY
        NAMES portaudio
        HINTS ${PKGCONFIG_PORTAUDIO_LIBDIR} ${PKGCONFIG_PORTAUDIO_LIBRARY_DIRS}
        PATHS
          /usr/lib
          /usr/local/lib
          /opt/local/lib
          /sw/lib
          /usr/local/mingw/i686-w64-mingw32/lib
          C:/MinGW/msys/1.0/usr/lib
          C:/MinGW/msys/1.0/usr/local/lib)

endif()

find_package(PackageHandleStandardArgs)
find_package_handle_standard_args(PortAudio DEFAULT_MSG PORTAUDIO_INCLUDE_DIR PORTAUDIO_LIBRARY)

if(PORTAUDIO_FOUND)
    set(PORTAUDIO_INCLUDE_DIRS ${PORTAUDIO_INCLUDE_DIR})
    set(PORTAUDIO_LIBRARIES ${PORTAUDIO_LIBRARY})
endif()

mark_as_advanced(PORTAUDIO_INCLUDE_DIRS PORTAUDIO_LIBRARIES)
