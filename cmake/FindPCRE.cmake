# - Find the PCRE library
#
# Usage:
#   find_package(PCRE [REQUIRED] [QUIET])
#
# It sets the following variables:
#   PCRE_FOUND               ... true if pcre is found on the system
#   PCRE_LIBRARIES           ... full path to pcre library
#   PCRE_INCLUDES            ... pcre include directory
#
# The following variables will be checked by the function
#   PCRE_ROOT               ... if set, the libraries are exclusively searched
#                               under this path

#If environment variable PCRE_DIR is specified, it has same effect as PCRE_ROOT
if(NOT PCRE_ROOT AND ENV{PCRE_DIR})
    set(PCRE_ROOT $ENV{PCRE_DIR})
endif()

if(PCRE_ROOT)

    find_path(PCRE_INCLUDE_DIR
        NAMES "pcre.h"
        PATHS ${PCRE_ROOT}
        PATH_SUFFIXES "include"
        NO_DEFAULT_PATH)

    find_library(PCRE_LIBRARY
        NAMES "pcre"
        PATHS ${PCRE_ROOT}
        PATH_SUFFIXES "lib" "lib64"
        NO_DEFAULT_PATH)

else()
    find_package(PkgConfig QUIET)
    if(PKG_CONFIG_FOUND)
        pkg_check_modules(PKGCONFIG_MODPLUG "pcre")
    endif()

    find_path(PCRE_INCLUDE_DIR
        NAMES "pcre.h"
        HINTS ${PKGCONFIG_PCRE_INCLUDE_DIRS}
        PATH_SUFFIXES "pcre"
        PATHS
           ${INCLUDE_INSTALL_DIR}
           /usr/include
           /usr/local/include
           /opt/local/include
           /sw/include)

    find_library(PCRE_LIBRARY
        NAMES "pcre"
        HINTS ${PKGCONFIG_PCRE_LIBRARY_DIRS}
        PATHS
          ${LIB_INSTALL_DIR}
          /usr/lib
          /usr/local/lib
          /opt/local/lib
          /sw/lib)

endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(PCRE DEFAULT_MSG PCRE_INCLUDE_DIR PCRE_LIBRARY)

if(PCRE_FOUND)
    set(PCRE_INCLUDES ${PCRE_INCLUDE_DIR})
    set(PCRE_LIBRARIES ${PCRE_LIBRARY})
endif()


mark_as_advanced(PCRE_INCLUDES PCRE_LIBRARIES)
