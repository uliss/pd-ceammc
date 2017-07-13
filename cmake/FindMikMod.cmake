# - Find the MikMod library
#
# Usage:
#   find_package(MikMod [REQUIRED] [QUIET])
#
# It sets the following variables:
#   MIKMOD_FOUND               ... true if mikmod is found on the system
#   MIKMOD_LIBRARIES           ... full path to mikmod library
#   MIKMOD_INCLUDES            ... mikmod include directory
#
# The following variables will be checked by the function
#   MIKMOD_ROOT               ... if set, the libraries are exclusively searched
#                               under this path

#If environment variable MIKMOD_DIR is specified, it has same effect as MIKMOD_ROOT
if(NOT MIKMOD_ROOT AND ENV{MIKMOD_DIR})
    set(MIKMOD_ROOT $ENV{MIKMOD_DIR})
endif()

if(MIKMOD_ROOT)

    find_path(MIKMOD_INCLUDE_DIR
        NAMES "mikmod.h"
        PATHS ${MIKMOD_ROOT}
        PATH_SUFFIXES "include"
        NO_DEFAULT_PATH)

    find_library(MIKMOD_LIBRARY
        NAMES "mikmod"
        PATHS ${MIKMOD_ROOT}
        PATH_SUFFIXES "lib" "lib64"
        NO_DEFAULT_PATH)

else()
    find_package(PkgConfig QUIET)
    if(PKG_CONFIG_FOUND)
        pkg_check_modules(PKGCONFIG_MIKMOD "libmikmod")
    endif()

    find_path(MIKMOD_INCLUDE_DIR
        NAMES "mikmod.h"
        HINTS ${PKGCONFIG_MIKMOD_INCLUDEDIR} ${PKGCONFIG_MIKMOD_INCLUDE_DIRS}
        PATHS
           ${INCLUDE_INSTALL_DIR}
           /usr/include
           /usr/local/include
           /opt/local/include
           /sw/include
           /usr/local/mingw/i686-w64-mingw32/include
           C:/MinGW/msys/1.0/usr/include
           C:/MinGW/msys/1.0/usr/local/include)

    find_library(MIKMOD_LIBRARY
        NAMES "mikmod"
        HINTS ${PKGCONFIG_MIKMOD_LIBDIR} ${PKGCONFIG_MIKMOD_LIBRARY_DIRS}
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
find_package_handle_standard_args(MIKMOD DEFAULT_MSG MIKMOD_INCLUDE_DIR MIKMOD_LIBRARY)

if(MIKMOD_FOUND)
    set(MIKMOD_INCLUDES ${MIKMOD_INCLUDE_DIR})
    set(MIKMOD_LIBRARIES ${MIKMOD_LIBRARY})
endif()


mark_as_advanced(MIKMOD_INCLUDES MIKMOD_LIBRARIES)
