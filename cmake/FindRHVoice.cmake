# - Find the RHVoice library
#
# Usage:
#   find_package(RHVoice [REQUIRED] [QUIET])
#
# It sets the following variables:
#   RHVoice_FOUND               ... true if RHVoice is found on the system
#   RHVoice_LIBRARIES           ... full path to RHVoice library
#   RHVoice_INCLUDES            ... RHVoice include directory
#
# The following variables will be checked by the function
#   RHVOICE_ROOT               ... if set, the libraries are exclusively searched
#                               under this path

#If environment variable RHVOICE_DIR is specified, it has same effect as RHVOICE_ROOT
if(NOT RHVOICE_ROOT AND ENV{RHVOICE_DIR})
    set(RHVOICE_ROOT $ENV{RHVOICE_DIR})
endif()

if(RHVOICE_ROOT)

    find_path(RHVOICE_INCLUDE_DIR
        NAMES "RHVoice.h"
        PATHS ${RHVOICE_ROOT}
        PATH_SUFFIXES "include"
        NO_DEFAULT_PATH)

    find_library(RHVOICE_LIBRARY
        NAMES "RHVoice"
        PATHS ${RHVOICE_ROOT}
        PATH_SUFFIXES "lib" "lib64"
        NO_DEFAULT_PATH)

else()

    find_path(RHVOICE_INCLUDE_DIR
        NAMES "RHVoice.h"
        PATH_SUFFIXES "include"
        PATHS
            $ENV{HOME}/.local)

    find_library(RHVOICE_LIBRARY
        NAMES "RHVoice"
        PATHS
            $ENV{HOME}/.local)

endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(RHVoice DEFAULT_MSG RHVOICE_INCLUDE_DIR RHVOICE_LIBRARY)

if(RHVoice_FOUND)
    set(RHVOICE_INCLUDE_DIRS ${RHVOICE_INCLUDE_DIR})
    set(RHVOICE_LIBRARIES    ${RHVOICE_LIBRARY})
endif()

mark_as_advanced(RHVOICE_INCLUDE_DIRS RHVOICE_LIBRARIES)
