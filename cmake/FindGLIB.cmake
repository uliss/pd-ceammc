# - Find the GLIB library
#
# Usage:
#   find_package(GLIB [REQUIRED] [QUIET])
#
# It sets the following variables:
#   GLIB_FOUND               ... true if glib is found on the system
#   GLIB_LIBRARIES           ... full path to glib library
#   GLIB_INCLUDES            ... glib include directory
#
# The following variables will be checked by the function
#   GLIB_ROOT               ... if set, the libraries are exclusively searched
#                               under this path

#If environment variable GLIBDIR is specified, it has same effect as GLIB_ROOT
if(NOT GLIB_ROOT AND ENV{GLIBDIR})
    set(GLIB_ROOT $ENV{GLIBDIR})
endif()

# root manually specified
if(GLIB_ROOT)

    find_path(GLIB_INCLUDE_DIR
        NAMES glib.h
        PATHS ${GLIB_ROOT}
        PATH_SUFFIXES include include/glib-2.0
        NO_DEFAULT_PATH)

    find_path(GLIB_CONFIG_INCLUDE_DIR
        NAMES glibconfig.h
        PATHS ${GLIB_ROOT}
        PATH_SUFFIXES include include/glib-2.0/include
        NO_DEFAULT_PATH)

    find_path(ICONV_INCLUDE_DIR
        NAMES iconv.h
        PATHS ${GLIB_ROOT}
        PATH_SUFFIXES include
        NO_DEFAULT_PATH)

    find_library(GLIB_LIBRARIES2
        NAMES glib-2.0
        PATHS ${GLIB_ROOT}
        PATH_SUFFIXES lib
        NO_DEFAULT_PATH
    )

    find_library(GTHREAD_LIBRARIES
        NAMES gthread-2.0
        PATHS ${GLIB_ROOT}
        PATH_SUFFIXES lib
        NO_DEFAULT_PATH
    )

    find_library(ICONV_LIBRARIES
        NAMES iconv
        PATHS ${GLIB_ROOT}
        PATH_SUFFIXES lib
        NO_DEFAULT_PATH
    )

    find_library(INTL_LIBRARIES
        NAMES intl
        PATH_SUFFIXES lib
        PATHS ${GLIB_ROOT}
        NO_DEFAULT_PATH
    )

else() # search via pkg-config

    find_package(PkgConfig QUIET)

    if(PKG_CONFIG_FOUND)
        pkg_check_modules(PKGCONFIG_GLIB glib-2.0)
    endif()

    set(_include_paths
        ${INCLUDE_INSTALL_DIR}
        /usr/include
        /opt/local/include
        /sw/include
        /usr/local/mingw/i686-w64-mingw32/include)

    set(_lib_paths
        ${LIB_INSTALL_DIR}
        /usr/lib
        /usr/local/lib
        /opt/local/lib
        /sw/lib
        /usr/local/mingw/i686-w64-mingw32/lib)

    find_path(GLIB_INCLUDE_DIR
        NAMES glib.h
        HINTS ${PKGCONFIG_GLIB_INCLUDEDIR} ${PKGCONFIG_GLIB_INCLUDE_DIRS}
        PATHS ${_include_paths}
        PATH_SUFFIXES glib-2.0
        NO_DEFAULT_PATH)

    find_path(ICONV_INCLUDE_DIR
        NAMES iconv.h
        HINTS ${PKGCONFIG_GLIB_INCLUDEDIR} ${PKGCONFIG_GLIB_INCLUDE_DIRS}
        PATHS ${_include_paths}
        NO_DEFAULT_PATH)

    find_path(GLIB_CONFIG_INCLUDE_DIR
        NAMES glibconfig.h
        HINTS ${PKGCONFIG_GLIB_INCLUDEDIR} ${PKGCONFIG_GLIB_INCLUDE_DIRS}
        PATHS ${_include_paths}
        PATH_SUFFIXES glib-2.0/include
        NO_DEFAULT_PATH)

    # libs
    find_library(GLIB_LIBRARIES2
        NAMES glib-2.0
        HINTS ${PKGCONFIG_GLIB_LIBDIR} ${PKGCONFIG_GLIB_LIBRARY_DIRS}
        PATHS ${_lib_paths}
        NO_DEFAULT_PATH
    )

    find_library(GTHREAD_LIBRARIES
        NAMES gthread-2.0
        HINTS ${PKGCONFIG_GLIB_LIBDIR} ${PKGCONFIG_GLIB_LIBRARY_DIRS}
        PATHS ${_lib_paths}
        NO_DEFAULT_PATH
    )

    find_library(ICONV_LIBRARIES
        NAMES libiconv iconv
        HINTS ${PKGCONFIG_GLIB_INCLUDEDIR} ${PKGCONFIG_GLIB_LIBRARY_DIRS}
        PATHS ${_lib_paths}
        NO_DEFAULT_PATH
    )

    find_library(INTL_LIBRARIES
        NAMES intl
        HINTS ${PKGCONFIG_GLIB_INCLUDEDIR} ${PKGCONFIG_GLIB_LIBRARY_DIRS}
        PATHS ${_lib_paths}
        NO_DEFAULT_PATH
    )

endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GLIB
    DEFAULT_MSG GLIB_INCLUDE_DIR GLIB_CONFIG_INCLUDE_DIR
    GLIB_LIBRARIES2 GTHREAD_LIBRARIES
    ICONV_INCLUDE_DIR)

if(GLIB_FOUND)
    set(GLIB_INCLUDES ${GLIB_INCLUDE_DIR} ${GLIB_CONFIG_INCLUDE_DIR} ${ICONV_INCLUDE_DIR})
    set(GLIB_LIBRARIES ${GLIB_LIBRARIES2} ${GTHREAD_LIBRARIES} ${ICONV_LIBRARIES} ${INTL_LIBRARIES})

    message(STATUS "glib-2.0 and iconv files: ")
    foreach(_inc ${GLIB_INCLUDES})
        message(STATUS "    ${_inc}")
    endforeach()

    foreach(_lib ${GLIB_LIBRARIES})
        message(STATUS "    ${_lib}")
    endforeach()
else()
    message(STATUS "glib-2.0 not found")
endif()

mark_as_advanced(GLIB_INCLUDES GLIB_LIBRARIES)
