# - Find the libLTC library
#
# Usage:
#   find_package(LTC [REQUIRED] [QUIET])
#
# It sets the following variables:
#   LTC_FOUND               ... true if libltc is found on the system
#   LTC_LIBRARIES           ... full path to libltc library
#   LTC_INCLUDES            ... libltc include directory
#
# The following variables will be checked by the function
#   LTC_ROOT               ... if set, the libraries are exclusively searched
#                               under this path

# If environment variable LTC_DIR is specified, it has same effect as LTC_ROOT
if(NOT LTC_ROOT AND ENV{LTC_DIR})
    set(LTC_ROOT $ENV{LTC_DIR})
endif()

if(LTC_ROOT)

    find_path(LTC_INCLUDE_DIR
        NAMES "ltc.h"
        HINTS ltc
        PATHS ${LTC_ROOT}
        PATH_SUFFIXES "include"
        NO_DEFAULT_PATH)

    find_library(LTC_LIBRARY
        NAMES "ltc"
        PATHS ${LTC_ROOT}
        PATH_SUFFIXES "lib" "lib64"
        NO_DEFAULT_PATH)

else()
    find_package(PkgConfig QUIET)
    if(PKG_CONFIG_FOUND)
        pkg_check_modules(PKGCONFIG_LTC "ltc" QUIET)
    endif()

    find_path(LTC_INCLUDE_DIR
        NAMES "ltc.h"
        HINTS ${PKGCONFIG_LTC_INCLUDEDIR} ${PKGCONFIG_LTC_INCLUDE_DIRS}
        PATH_SUFFIXES "libltc"
        PATHS
           ${INCLUDE_INSTALL_DIR}
           /usr/include
           /usr/local/include
           /opt/local/include
           /sw/include
           /usr/local/mingw/i686-w64-mingw32/include
           C:/MinGW/msys/1.0/usr/include
           C:/MinGW/msys/1.0/usr/local/include)

    find_library(LTC_LIBRARY
        NAMES "ltc"
        HINTS ${PKGCONFIG_LTC_LIBDIR} ${PKGCONFIG_LTC_LIBRARY_DIRS}
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
find_package_handle_standard_args(LTC DEFAULT_MSG LTC_INCLUDE_DIR LTC_LIBRARY)

if(LTC_FOUND)
    set(LTC_INCLUDE_DIRS ${LTC_INCLUDE_DIR})
    set(LTC_LIBRARIES ${LTC_LIBRARY})
endif()


mark_as_advanced(LTC_INCLUDES LTC_LIBRARIES)
