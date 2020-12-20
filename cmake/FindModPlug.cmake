# - Find the ModPlug library
#
# Usage:
#   find_package(ModPlug [REQUIRED] [QUIET])
#
# It sets the following variables:
#   ModPlug_FOUND               ... true if modplug is found on the system
#   MODPLUG_LIBRARIES           ... full path to modplug library
#   MODPLUG_INCLUDES            ... modplug include directory
#
# The following variables will be checked by the function
#   MODPLUG_ROOT               ... if set, the libraries are exclusively searched
#                               under this path

#If environment variable MODPLUG_DIR is specified, it has same effect as MODPLUG_ROOT
if(NOT MODPLUG_ROOT AND ENV{MODPLUG_DIR})
    set(MODPLUG_ROOT $ENV{MODPLUG_DIR})
endif()

if(MODPLUG_ROOT)

    find_path(MODPLUG_INCLUDE_DIR
        NAMES "modplug.h"
        HINTS libmodplug
        PATHS ${MODPLUG_ROOT}
        PATH_SUFFIXES "include"
        NO_DEFAULT_PATH)

    find_library(MODPLUG_LIBRARY
        NAMES "modplug"
        PATHS ${MODPLUG_ROOT}
        PATH_SUFFIXES "lib" "lib64"
        NO_DEFAULT_PATH)

else()
    find_package(PkgConfig QUIET)
    if(PKG_CONFIG_FOUND)
        pkg_check_modules(PKGCONFIG_MODPLUG "libmodplug" QUIET)
    endif()

    find_path(MODPLUG_INCLUDE_DIR
        NAMES "modplug.h"
        HINTS ${PKGCONFIG_MODPLUG_INCLUDEDIR} ${PKGCONFIG_MODPLUG_INCLUDE_DIRS}
        PATH_SUFFIXES "libmodplug"
        PATHS
           ${INCLUDE_INSTALL_DIR}
           /usr/include
           /usr/local/include
           /opt/local/include
           /sw/include
           /usr/local/mingw/i686-w64-mingw32/include
           C:/MinGW/msys/1.0/usr/include
           C:/MinGW/msys/1.0/usr/local/include)

    find_library(MODPLUG_LIBRARY
        NAMES "modplug"
        HINTS ${PKGCONFIG_MODPLUG_LIBDIR} ${PKGCONFIG_MODPLUG_LIBRARY_DIRS}
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
find_package_handle_standard_args(ModPlug DEFAULT_MSG MODPLUG_INCLUDE_DIR MODPLUG_LIBRARY)

if(ModPlug_FOUND)
    set(MODPLUG_INCLUDES ${MODPLUG_INCLUDE_DIR})
    set(MODPLUG_LIBRARIES ${MODPLUG_LIBRARY})
endif()


mark_as_advanced(MODPLUG_INCLUDES MODPLUG_LIBRARIES)
