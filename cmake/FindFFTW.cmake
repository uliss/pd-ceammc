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

if(POLICY CMP0074)
    #policy for <PackageName>_ROOT variables
    cmake_policy(SET CMP0074 NEW)
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
        PATH_SUFFIXES "lib" "lib64" "lib/arm-linux-gnueabihf"
        NO_DEFAULT_PATH)

else()
    find_package(PkgConfig QUIET)
    if(PKG_CONFIG_FOUND)
        pkg_check_modules(PKGCONFIG_FFTW "fftw3f")
    endif()

    find_path(FFTW_INCLUDE_DIR
        NAMES "fftw3.h"
        HINTS ${PKGCONFIG_FFTW_INCLUDEDIR} ${PKGCONFIG_FFTW_INCLUDE_DIRS}
        PATHS
           ${INCLUDE_INSTALL_DIR}
           /usr/include
           /usr/local/include
           /opt/local/include
           /sw/include
           /usr/local/mingw/i686-w64-mingw32/include
           C:/MinGW/msys/1.0/usr/include
           C:/MinGW/msys/1.0/usr/local/include)

    find_library(FFTW_FLOAT_LIBRARY
        NAMES "fftw3f"
        HINTS ${PKGCONFIG_FFTW_LIBDIR} ${PKGCONFIG_FFTW_LIBRARY_DIRS}
	PATH_SUFFIXES lib lib/arm-linux-gnueabihf
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
find_package_handle_standard_args(FFTW DEFAULT_MSG FFTW_INCLUDE_DIR FFTW_FLOAT_LIBRARY)

if(FFTW_FOUND)
    set(FFTW_INCLUDES ${FFTW_INCLUDE_DIR})
    set(FFTW_LIBRARIES ${FFTW_FLOAT_LIBRARY})
endif()


mark_as_advanced(FFTW_INCLUDES FFTW_LIBRARIES FFTWF_LIB)
