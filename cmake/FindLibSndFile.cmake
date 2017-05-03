# - Try to find libsndfile
# Once done, this will define
#
#  LIBSNDFILE_FOUND - system has libsndfile
#  LIBSNDFILE_INCLUDE_DIRS - the libsndfile include directories
#  LIBSNDFILE_LIBRARIES - link these to use libsndfile

#If environment variable LIBSNDFILE_DIR is specified, it has same effect as LIBSNDFILE_ROOT
if(NOT LIBSNDFILE_ROOT AND ENV{LIBSNDFILE_DIR})
    set(LIBSNDFILE_ROOT $ENV{LIBSNDFILE_DIR})
endif()

# Use pkg-config to get hints about paths
find_package(PkgConfig QUIET)
if(PKG_CONFIG_FOUND)
    pkg_check_modules(PKGCONFIG_LIBSNDFILE sndfile)
endif()

# libsndfile source root specified
if(LIBSNDFILE_ROOT)

    find_path(LIBSNDFILE_INCLUDE_DIR
        NAMES sndfile.h
        PATHS ${LIBSNDFILE_ROOT}
        PATH_SUFFIXES "include"
        NO_DEFAULT_PATH)

    find_library(LIBSNDFILE_LIBRARY
        NAMES sndfile libsndfile-1
        PATHS ${LIBSNDFILE_ROOT}
        PATH_SUFFIXES "lib" "lib64"
        NO_DEFAULT_PATH)

elseif(PKGCONFIG_LIBSNDFILE_FOUND)

    set(LIBSNDFILE_INCLUDE_DIR ${PKGCONFIG_LIBSNDFILE_INCLUDE_DIRS})
    set(LIBSNDFILE_LIBRARY ${PKGCONFIG_LIBSNDFILE_LIBRARIES})

else()

    find_path(LIBSNDFILE_INCLUDE_DIR
        NAMES sndfile.h
        PATHS
          ${INCLUDE_INSTALL_DIR}
          C:/MinGW/msys/1.0/usr/lib
          C:/MinGW/msys/1.0/usr/local/lib)

    find_library(LIBSNDFILE_LIBRARY
          NAMES sndfile libsndfile-1
          PATHS
            ${LIB_INSTALL_DIR}
            C:/MinGW/msys/1.0/usr/lib
            C:/MinGW/msys/1.0/usr/local/lib)

endif()

find_package(PackageHandleStandardArgs)
find_package_handle_standard_args(LibSndFile DEFAULT_MSG LIBSNDFILE_INCLUDE_DIR LIBSNDFILE_LIBRARY)

if(LIBSNDFILE_FOUND)
    set(LIBSNDFILE_LIBRARIES ${LIBSNDFILE_LIBRARY})
    set(LIBSNDFILE_INCLUDE_DIRS ${LIBSNDFILE_INCLUDE_DIR})
endif()

mark_as_advanced(LIBSNDFILE_LIBRARY LIBSNDFILE_LIBRARIES LIBSNDFILE_INCLUDE_DIR LIBSNDFILE_INCLUDE_DIRS)
