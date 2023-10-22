# - Find the RubberBand library
#
# Usage:
#   find_package(RubberBand [REQUIRED] [QUIET])
#
# It sets the following variables:
#   RubberBand_FOUND               ... true if libltc is found on the system
#   RUBBERBAND_LIBRARIES           ... full path to libltc library
#   RUBBERBAND_INCLUDES            ... libltc include directory
#
# The following variables will be checked by the function
#   RUBBERBAND_ROOT               ... if set, the libraries are exclusively searched
#                               under this path

# If environment variable RUBBERBAND_DIR is specified, it has same effect as RUBBERBAND_ROOT
if(NOT RUBBERBAND_ROOT AND ENV{RUBBERBAND_DIR})
    set(RUBBERBAND_ROOT $ENV{RUBBERBAND_DIR})
endif()

if(RUBBERBAND_ROOT)

    find_path(RUBBERBAND_INCLUDE_DIR
        NAMES "rubberband-c.h"
        HINTS rubberband
        PATHS ${RUBBERBAND_ROOT}
        PATH_SUFFIXES "include"
        NO_DEFAULT_PATH)

    find_library(RUBBERBAND_LIBRARY
        NAMES "rubberband"
        PATHS ${RUBBERBAND_ROOT}
        PATH_SUFFIXES "lib" "lib64"
        NO_DEFAULT_PATH)

else()
    find_package(PkgConfig QUIET)
    if(PKG_CONFIG_FOUND)
        pkg_check_modules(PKGCONFIG_RUBBERBAND "rubberband" QUIET)
    endif()

    find_path(RUBBERBAND_INCLUDE_DIR
        NAMES "rubberband-c.h"
        HINTS ${PKGCONFIG_RUBBERBAND_INCLUDEDIR} ${PKGCONFIG_RUBBERBAND_INCLUDE_DIRS}
        PATH_SUFFIXES "rubberband"
        PATHS
           ${INCLUDE_INSTALL_DIR}
           /usr/include
           /usr/local/include
           /opt/local/include
           /sw/include
           /usr/local/mingw/i686-w64-mingw32/include
           C:/MinGW/msys/1.0/usr/include
           C:/MinGW/msys/1.0/usr/local/include)

    find_library(RUBBERBAND_LIBRARY
        NAMES "rubberband"
        HINTS ${PKGCONFIG_RUBBERBAND_LIBDIR} ${PKGCONFIG_RUBBERBAND_LIBRARY_DIRS}
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
find_package_handle_standard_args(RubberBand DEFAULT_MSG RUBBERBAND_INCLUDE_DIR RUBBERBAND_LIBRARY)

if(RUBBERBAND_FOUND)
    set(RUBBERBAND_INCLUDE_DIRS ${RUBBERBAND_INCLUDE_DIR})
    set(RUBBERBAND_LIBRARIES ${RUBBERBAND_LIBRARY})
endif()


mark_as_advanced(RUBBERBAND_INCLUDES RUBBERBAND_LIBRARIES)
