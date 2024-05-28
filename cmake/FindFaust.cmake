# - Find the Faust library
#
# Usage:
#   find_package(Faust [REQUIRED] [QUIET])
#
# It sets the following variables:
#   Faust_FOUND               ... true if libfaust is found on the system
#   Faust_LIBRARIES           ... full path to libfaust library
#   Faust_INCLUDE_DIRS        ... libfaust include directory
#
# The following variables will be checked by the function
#   Faust_ROOT               ... if set, the libraries are exclusively searched
#                               under this path

# If environment variable FAUST_DIR is specified, it has same effect as LTC_ROOT
if(NOT Faust_ROOT AND ENV{FAUST_DIR})
    set(Faust_ROOT $ENV{FAUST_DIR})
endif()

if(Faust_ROOT)

    find_path(FAUST_INCLUDE_DIR
        NAMES "faust/dsp/faust-engine.h"
        PATHS ${Faust_ROOT}
        PATH_SUFFIXES "include"
        NO_DEFAULT_PATH)

    find_library(FAUST_LIBRARY
        NAMES "faust" "libfaust"
        PATHS ${Faust_ROOT}
        PATH_SUFFIXES "lib" "lib64"
        NO_DEFAULT_PATH)

else()

    find_path(FAUST_INCLUDE_DIR
        NAMES "faust/dsp/faust-engine.h"
        HINTS C:/msys64/opt/local/include
        PATH_SUFFIXES "include"
        PATHS
           /usr/include
           /usr/local/include
           /opt/local/include)

    find_library(FAUST_LIBRARY
        NAMES "faust" "libfaust"
        HINTS C:/msys64/opt/local/lib
        PATHS
          ${LIB_INSTALL_DIR}
          /usr/lib
          /usr/local/lib
          /opt/local/lib
          /opt/local/bin)


    find_path(FAUST_DSP_LIBRARY
        NAMES stdfaust.lib
        HINTS C:/msys64/opt/local/share
        PATH_SUFFIXES "faust"
        PATHS
          ${LIB_INSTALL_DIR}
          /usr/lib
          /usr/local/lib
          /opt/local/lib
          /opt/local/bin)

    if(FAUST_DSP_LIBRARY)
        get_filename_component(FAUST_DSP_LIBRARY_DIR ${FAUST_DSP_LIBRARY} DIRECTORY)
    endif()

endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Faust DEFAULT_MSG FAUST_INCLUDE_DIR FAUST_LIBRARY)

if(Faust_FOUND)
    set(Faust_INCLUDE_DIRS ${FAUST_INCLUDE_DIR})
    set(Faust_LIBRARIES ${FAUST_LIBRARY})
    message(STATUS "Found Faust DSP library: ${FAUST_DSP_LIBRARY_DIR}")
endif()

mark_as_advanced(Faust_INCLUDE_DIRS Faust_LIBRARIES)
