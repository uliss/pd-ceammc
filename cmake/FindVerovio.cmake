# - Find the verovio library
#
# Usage:
#   find_package(Verovio [REQUIRED] [QUIET])
#
# It sets the following variables:
#   Verovio_FOUND               ... true if verovio is found on the system
#   VEROVIO_LIBRARIES           ... full path to verovio library
#   VEROVIO_INCLUDE_DIRS        ... verovio include directory
#
# The following variables will be checked by the function
#   VEROVIO_ROOT               ... if set, the libraries are exclusively searched
#                               under this path

# If environment variable VEROVIO_DIR is specified, it has same effect as VEROVIO_ROOT
if(NOT VEROVIO_ROOT AND ENV{VEROVIO_DIR})
    set(VEROVIO_ROOT $ENV{VEROVIO_DIR})
endif()


if(VEROVIO_ROOT)
    find_path(VEROVIO_INCLUDE_DIR
        NAMES "verovio/vrv.h"
        PATHS ${VEROVIO_ROOT}
        PATH_SUFFIXES "include"
        NO_DEFAULT_PATH)

    find_library(VEROVIO_LIBRARY
        NAMES "verovio"
        PATHS ${VEROVIO_ROOT}
        PATH_SUFFIXES "lib"
        NO_DEFAULT_PATH)

    find_path(VEROVIO_DATA
        NAMES "Bravura.xml"
        PATHS ${VEROVIO_ROOT}
        PATH_SUFFIXES "share/verovio"
        NO_DEFAULT_PATH)

else()

    find_path(VEROVIO_INCLUDE_DIR
        NAMES "verovio/vrv.h"
        PATHS $ENV{HOME}/.local/verovio
        PATH_SUFFIXES "include"
        NO_DEFAULT_PATH)

    find_library(VEROVIO_LIBRARY
        NAMES "verovio"
        PATHS $ENV{HOME}/.local/verovio
        PATH_SUFFIXES "lib"
        NO_DEFAULT_PATH)

    find_path(VEROVIO_DATA
        NAMES "Bravura.xml"
        PATHS $ENV{HOME}/.local/verovio
        PATH_SUFFIXES "share/verovio"
        NO_DEFAULT_PATH)

endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Verovio DEFAULT_MSG VEROVIO_INCLUDE_DIR VEROVIO_LIBRARY)

if(Verovio_FOUND)
    set(VEROVIO_INCLUDE_DIRS ${VEROVIO_INCLUDE_DIR})
    set(VEROVIO_LIBRARIES ${VEROVIO_LIBRARY})
endif()

mark_as_advanced(VEROVIO_INCLUDES VEROVIO_LIBRARIES)
