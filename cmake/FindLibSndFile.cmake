# - Try to find libsndfile
# Once done, this will define
#
#  LIBSNDFILE_FOUND - system has libsndfile
#  LIBSNDFILE_INCLUDE_DIRS - the libsndfile include directories
#  LIBSNDFILE_LIBRARIES - link these to use libsndfile

#If environment variable FFTWDIR is specified, it has same effect as FFTW_ROOT
if(NOT LIBSNDFILE_ROOT AND ENV{LIBSNDFILE_DIR})
  set(LIBSNDFILE_ROOT $ENV{LIBSNDFILE_DIR})
endif()

# Use pkg-config to get hints about paths
find_package(PkgConfig QUIET)
if(PKG_CONFIG_FOUND)
    pkg_check_modules(LIBSNDFILE_PKGCONF sndfile)
endif()

# Include dir
find_path(LIBSNDFILE_INCLUDE_DIR
    NAMES sndfile.h
    PATHS ${LIBSNDFILE_PKGCONF_INCLUDE_DIRS}
)

message(STATUS "pkgconfig: ${LIBSNDFILE_PKGCONF_LIBRARY_DIRS}")

# Library
if(LIBSNDFILE_ROOT)
    find_library(LIBSNDFILE_LIBRARY
        NAMES sndfile libsndfile-1
        PATHS
        ${LIBSNDFILE_PKGCONF_LIBRARY_DIRS}
        PATH_SUFFIXES "lib" "lib64"
        NO_DEFAULT_PATH
    )
else()
    find_library(LIBSNDFILE_LIBRARY
        NAMES sndfile libsndfile-1
        PATHS
        ${LIBSNDFILE_PKGCONF_LIBRARY_DIRS}
        /usr/lib
        /usr/local/lib
        /opt/local/lib
        /usr/lib/x86_64-linux-gnu
        /usr/lib/i386-linux-gnu
    )
endif()

find_package(PackageHandleStandardArgs)
find_package_handle_standard_args(LibSndFile  DEFAULT_MSG  LIBSNDFILE_LIBRARY LIBSNDFILE_INCLUDE_DIR)

if(LIBSNDFILE_FOUND)
  set(LIBSNDFILE_LIBRARIES ${LIBSNDFILE_LIBRARY})
  set(LIBSNDFILE_INCLUDE_DIRS ${LIBSNDFILE_INCLUDE_DIR})
endif(LIBSNDFILE_FOUND)

mark_as_advanced(LIBSNDFILE_LIBRARY LIBSNDFILE_LIBRARIES LIBSNDFILE_INCLUDE_DIR LIBSNDFILE_INCLUDE_DIRS)
