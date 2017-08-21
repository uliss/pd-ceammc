# - Find the LeapMotion SDK
#
# Usage:
#   find_package(LeapMotion [REQUIRED] [QUIET])
#
# It sets the following variables:
#   LEAPMOTION_FOUND               ... true if leapmotion is found on the system
#   LEAPMOTION_LIBRARIES           ... full path to leapmotion library
#   LEAPMOTION_INCLUDES            ... leapmotion include directory
#
# The following variables will be checked by the function
#   LEAPMOTION_ROOT               ... if set, the libraries are exclusively searched
#                               under this path

#If environment variable MODPLUG_DIR is specified, it has same effect as MODPLUG_ROOT
if(NOT LEAPMOTION_ROOT AND ENV{LEAPMOTION_DIR})
    set(LEAPMOTION_ROOT $ENV{LEAPMOTION_DIR})
endif()

include(FindPackageHandleStandardArgs)

if(LEAPMOTION_ROOT)

    find_path(LEAPMOTION_INCLUDE_DIR
        NAMES "Leap.h"
        PATHS ${LEAPMOTION_ROOT}
        PATH_SUFFIXES "include"
        NO_DEFAULT_PATH)

    find_library(LEAPMOTION_LIBRARY
        NAMES "Leap"
        PATHS ${LEAPMOTION_ROOT}
        PATH_SUFFIXES "lib" "lib64"
        NO_DEFAULT_PATH)

    find_package_handle_standard_args(LEAPMOTION DEFAULT_MSG LEAPMOTION_INCLUDE_DIR LEAPMOTION_LIBRARY)
    
    if(LEAPMOTION_FOUND)
        set(LEAPMOTION_INCLUDES ${LEAPMOTION_INCLUDE_DIR})
        set(LEAPMOTION_LIBRARIES ${LEAPMOTION_LIBRARY})
    endif()
     
    mark_as_advanced(LEAPMOTION_INCLUDES LEAPMOTION_LIBRARIES)
    
endif()

