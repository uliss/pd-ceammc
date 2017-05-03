#  PORTAUDIO_FOUND - system has libportaudio
#  PORTAUDIO_INCLUDE_DIRS - the libportaudio include directory
#  PORTAUDIO_LIBRARIES - Link these to use libportaudio
#  PORTAUDIO_ROOT - manually set where portaudio exists

# Check if we can use PkgConfig
find_package(PkgConfig)

# portaudio source root specified
if(PORTAUDIO_ROOT)

    find_path(PORTAUDIO_INCLUDE_DIR
        NAMES portaudio.h
        PATHS ${PORTAUDIO_ROOT}
        PATH_SUFFIXES "include"
        NO_DEFAULT_PATH)

    find_library(PORTAUDIO_LIBRARY
        NAMES portaudio libportaudio
        PATHS ${PORTAUDIO_ROOT}
        PATH_SUFFIXES "lib" "lib64"
        NO_DEFAULT_PATH)

elseif(PKG_CONFIG_FOUND)

    pkg_check_modules(PKGCONFIG_PORTAUDIO portaudio-2.0)

    if(PKGCONFIG_PORTAUDIO_FOUND)
        set(PORTAUDIO_INCLUDE_DIR ${PKGCONFIG_PORTAUDIO_INCLUDE_DIRS})
        set(PORTAUDIO_LIBRARY ${PKGCONFIG_PORTAUDIO_LIBRARIES})
    endif()

else()

    find_path(PORTAUDIO_INCLUDE_DIR
        NAMES portaudio.h
        PATHS
            /usr/include
            /usr/local/include
            /opt/local/include
            /sw/include
            /usr/local/mingw/i686-w64-mingw32
            C:/MinGW/msys/1.0/usr/lib
            C:/MinGW/msys/1.0/usr/local/lib
      )

      find_library(PORTAUDIO_LIBRARY
        NAMES portaudio
        PATHS
            /usr/lib
            /usr/local/lib
            /opt/local/lib
            /sw/lib
            /usr/local/mingw/i686-w64-mingw32
            C:/MinGW/msys/1.0/usr/lib
            C:/MinGW/msys/1.0/usr/local/lib
      )
endif()

find_package(PackageHandleStandardArgs)
find_package_handle_standard_args(PortAudio DEFAULT_MSG PORTAUDIO_INCLUDE_DIR PORTAUDIO_LIBRARY)

if(PORTAUDIO_FOUND)
    set(PORTAUDIO_INCLUDE_DIRS ${PORTAUDIO_INCLUDE_DIR})
    set(PORTAUDIO_LIBRARIES ${PORTAUDIO_LIBRARY})
endif()

mark_as_advanced(PORTAUDIO_INCLUDE_DIRS PORTAUDIO_LIBRARIES)
