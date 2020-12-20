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

#If environment variable LEAPMOTION_DIR is specified, it has same effect as LEAPMOTION_ROOT
if(NOT LEAPMOTION_ROOT AND ENV{LEAPMOTION_DIR})
    set(LEAPMOTION_ROOT $ENV{LEAPMOTION_DIR})
endif()

include(FindPackageHandleStandardArgs)

if(LEAPMOTION_ROOT)
    message(STATUS "LeapMotion: root specified at ${LEAPMOTION_ROOT}")

    find_path(LEAPMOTION_INCLUDE_DIR
        NAMES "Leap.h"
        PATHS ${LEAPMOTION_ROOT}
        HINTS{}
        PATH_SUFFIXES "include")

    message(STATUS ${LEAPMOTION_ROOT}/lib/x86)

    find_library(LEAPMOTION_LIBRARY
        NAMES Leap Leap.dll
        PATHS ${LEAPMOTION_ROOT}
              ${LEAPMOTION_ROOT}/lib/x86
              ${LEAPMOTION_ROOT}/lib/x64
        PATH_SUFFIXES "lib" "lib64")

    find_package_handle_standard_args(LEAPMOTION DEFAULT_MSG LEAPMOTION_INCLUDE_DIR LEAPMOTION_LIBRARY)
    
    if(LEAPMOTION_FOUND)
        set(LEAPMOTION_INCLUDES ${LEAPMOTION_INCLUDE_DIR})
        set(LEAPMOTION_LIBRARIES ${LEAPMOTION_LIBRARY})
    endif()
     
    mark_as_advanced(LEAPMOTION_INCLUDES LEAPMOTION_LIBRARIES)
    
endif()

