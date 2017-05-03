# - Find the FFTW library
#
# Usage:
#   find_package(FFTW [REQUIRED] [QUIET])
#     
# It sets the following variables:
#   FFTW_FOUND               ... true if fftw is found on the system
#   FFTW_LIBRARIES           ... full path to fftw library
#   FFTW_INCLUDES            ... fftw include directory
#
# The following variables will be checked by the function
#   FFTW_ROOT               ... if set, the libraries are exclusively searched
#                               under this path

#If environment variable FFTWDIR is specified, it has same effect as FFTW_ROOT
if(NOT FFTW_ROOT AND ENV{FFTWDIR})
    set(FFTW_ROOT $ENV{FFTWDIR})
endif()

# Check if we can use PkgConfig
find_package(PkgConfig QUIET)
if(PKG_CONFIG_FOUND)
    pkg_check_modules(PKGCONFIG_FFTW "fftw3")
endif()

if(FFTW_ROOT)

    find_path(FFTW_INCLUDE_DIR
        NAMES "fftw3.h"
        PATHS ${FFTW_ROOT}
        PATH_SUFFIXES "include"
        NO_DEFAULT_PATH)

    find_library(FFTW_FLOAT_LIBRARY
        NAMES "fftw3f"
        PATHS ${FFTW_ROOT}
        PATH_SUFFIXES "lib" "lib64"
        NO_DEFAULT_PATH)

elseif(PKGCONFIG_FFTW_FOUND)

    set(FFTW_INCLUDE_DIR ${PKGCONFIG_FFTW_INCLUDE_DIRS})
    set(FFTW_FLOAT_LIBRARY ${PKGCONFIG_FFTW_LIBRARIES})

else()

    find_path(FFTW_INCLUDE_DIR
        NAMES "fftw3.h"
        PATHS
           ${INCLUDE_INSTALL_DIR}
           C:/MinGW/msys/1.0/include
           C:/MinGW/msys/1.0/local/include)

    find_library(FFTW_FLOAT_LIBRARY
        NAMES "fftw3f"
        PATHS
          ${LIB_INSTALL_DIR}
          /usr/local/lib
          C:/MinGW/msys/1.0/lib
          C:/MinGW/msys/1.0/local/lib)

endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(FFTW DEFAULT_MSG FFTW_INCLUDE_DIR FFTW_FLOAT_LIBRARY)

if(FFTW_FOUND)
    set(FFTW_INCLUDES ${FFTW_INCLUDE_DIR})
    set(FFTW_LIBRARIES ${FFTW_FLOAT_LIBRARY})
endif()


mark_as_advanced(FFTW_INCLUDES FFTW_LIBRARIES FFTWF_LIB)

