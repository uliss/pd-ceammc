# - Try to find libmagic
# Once done, this will define
#
#  LibMagic_FOUND - system has libmagic
#  LIBMAGIC_INCLUDE_DIRS - the libmagic include directories
#  LIBMAGIC_LIBRARIES - link these to use libmagic

# Use pkg-config to get hints about paths
find_package(PkgConfig QUIET)
if(PKG_CONFIG_FOUND)
    pkg_check_modules(PKGCONFIG_LIBMAGIC libmagic)
endif()

find_path(LIBMAGIC_INCLUDE_DIR
    NAMES magic.h
    HINTS ${PKGCONFIG_LIBMAGIC_INCLUDEDIR} ${PKGCONFIG_LIBMAGIC_INCLUDE_DIRS}
    PATHS
      ${INCLUDE_INSTALL_DIR}
      /usr/include
      /usr/local/include
      /opt/local/include)

find_library(LIBMAGIC_LIBRARY
    NAMES magic libmagic
    HINTS ${PKGCONFIG_LIBMAGIC_LIBDIR} ${PKGCONFIG_LIBMAGIC_LIBRARY_DIRS}
    PATH_SUFFIXES lib lib/arm-linux-gnueabihf
    PATHS
      ${LIB_INSTALL_DIR}
      /usr/lib
      /usr/local/lib
      /opt/local/lib
      /sw/lib)

find_package(PackageHandleStandardArgs)
find_package_handle_standard_args(LibMagic DEFAULT_MSG LIBMAGIC_INCLUDE_DIR LIBMAGIC_LIBRARY)

if(LibMagic_FOUND)
    set(LIBMAGIC_LIBRARIES ${LIBMAGIC_LIBRARY})
    set(LIBMAGIC_INCLUDE_DIRS ${LIBMAGIC_INCLUDE_DIR})
endif()

mark_as_advanced(LIBMAGIC_LIBRARY LIBMAGIC_LIBRARIES LIBMAGIC_INCLUDE_DIR LIBMAGIC_INCLUDE_DIRS)
