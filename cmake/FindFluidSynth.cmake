# - Find the FluidSynth library
#
# Usage:
#   find_package(FluidSynth [REQUIRED] [QUIET])
#
# It sets the following variables:
#   FLUIDSYNTH_FOUND               ... true if FluidSynth is found on the system
#   FLUIDSYNTH_LIBRARIES           ... full path to FluidSynth library
#   FLUIDSYNTH_INCLUDES            ... FluidSynth include directory
#
# The following variables will be checked by the function
#   FLUIDSYNTH_ROOT               ... if set, the libraries are exclusively searched
#                               under this path

#If environment variable FLUIDSYNTH_DIR is specified, it has same effect as FLUIDSYNTH_ROOT
if(NOT FLUIDSYNTH_ROOT AND ENV{FLUIDSYNTH_DIR})
    set(FLUIDSYNTH_ROOT $ENV{FLUIDSYNTH_DIR})
endif()

if(FLUIDSYNTH_ROOT)

    find_path(FLUIDSYNTH_INCLUDE_DIR
        NAMES "fluidsynth.h"
        HINTS libfluidsynth
        PATHS ${FLUIDSYNTH_ROOT}
        PATH_SUFFIXES "include"
        NO_DEFAULT_PATH)

    find_library(FLUIDSYNTH_LIBRARY
        NAMES "fluidsynth"
        PATHS ${FLUIDSYNTH_ROOT}
        PATH_SUFFIXES "lib" "lib64"
        NO_DEFAULT_PATH)

else()
    find_package(PkgConfig QUIET)
    if(PKG_CONFIG_FOUND)
        pkg_check_modules(PKGCONFIG_FLUIDSYNTH "fluidsynth")
    endif()

    find_path(FLUIDSYNTH_INCLUDE_DIR
        NAMES "fluidsynth.h"
        HINTS ${PKGCONFIG_FLUIDSYNTH_INCLUDEDIR} ${PKGCONFIG_FLUIDSYNTH_INCLUDE_DIRS}
        PATH_SUFFIXES "libfluidsynth"
        PATHS
           ${INCLUDE_INSTALL_DIR}
           /usr/include
           /usr/local/include
           /opt/local/include
           /sw/include
           /usr/local/mingw/i686-w64-mingw32/include
           C:/MinGW/msys/1.0/usr/include
           C:/MinGW/msys/1.0/usr/local/include)

    find_library(FLUIDSYNTH_LIBRARY
        NAMES "fluidsynth"
        HINTS ${PKGCONFIG_FLUIDSYNTH_LIBDIR} ${PKGCONFIG_FLUIDSYNTH_LIBRARY_DIRS}
        PATHS
          ${LIB_INSTALL_DIR}
          /usr/lib
          /usr/local/lib
          /opt/local/lib
          /sw/lib
          /usr/local/mingw/i686-w64-mingw32/lib
          C:/MinGW/msys/1.0/usr/lib
          C:/MinGW/msys/1.0/usr/local/lib)

endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(FLUIDSYNTH DEFAULT_MSG FLUIDSYNTH_INCLUDE_DIR FLUIDSYNTH_LIBRARY)

if(FLUIDSYNTH_FOUND)
    set(FLUIDSYNTH_INCLUDES ${FLUIDSYNTH_INCLUDE_DIR})
    set(FLUIDSYNTH_LIBRARIES ${FLUIDSYNTH_LIBRARY})
endif()


mark_as_advanced(FLUIDSYNTH_INCLUDES FLUIDSYNTH_LIBRARIES)
