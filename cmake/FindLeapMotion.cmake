# - Find the LeapMotion SDK
#
# Usage:
#   find_package(LeapMotion [REQUIRED] [QUIET])
#
# It sets the following variables:
#   LeapMotion_FOUND               ... true if leapmotion is found on the system
#   LEAPMOTION_LIBRARIES           ... full path to leapmotion library
#   LEAPMOTION_INCLUDES            ... leapmotion include directory
#
# The following variables will be checked by the function
#   LeapMotion_ROOT               ... if set, the libraries are exclusively searched
#                               under this path

#If environment variable LEAPMOTION_DIR is specified, it has same effect as LEAPMOTION_ROOT
if(NOT LeapMotion_ROOT AND ENV{LEAPMOTION_DIR})
    set(LeapMotion_ROOT $ENV{LEAPMOTION_DIR})
endif()

include(FindPackageHandleStandardArgs)

if(LeapMotion_ROOT)
    message(STATUS "LeapMotion: root specified at ${LeapMotion_ROOT}")

    find_path(LEAPMOTION_INCLUDE_DIR
        NAMES "Leap.h"
        PATHS ${LeapMotion_ROOT}
        HINTS{}
        PATH_SUFFIXES "include")

    message(STATUS ${LeapMotion_ROOT}/lib/x86)

    find_library(LEAPMOTION_LIBRARY
        NAMES Leap Leap.dll
        PATHS ${LeapMotion_ROOT}
              ${LeapMotion_ROOT}/lib/x86
              ${LeapMotion_ROOT}/lib/x64
        PATH_SUFFIXES "lib" "lib64")

    find_package_handle_standard_args(LeapMotion DEFAULT_MSG LEAPMOTION_INCLUDE_DIR LEAPMOTION_LIBRARY)
    
    if(LeapMotion_FOUND)
        set(LEAPMOTION_INCLUDES ${LEAPMOTION_INCLUDE_DIR})
        set(LEAPMOTION_LIBRARIES ${LEAPMOTION_LIBRARY})
    endif()
     
    mark_as_advanced(LEAPMOTION_INCLUDES LEAPMOTION_LIBRARIES)
    
endif()

