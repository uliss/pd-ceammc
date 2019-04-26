if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR
        "CMake generation for pure-data is not allowed within the source directory!"
        "\n Remove the CMakeCache.txt file and try again from another folder, e.g.:"
        "\n "
        "\n rm CMakeCache.txt"
        "\n cd .."
        "\n mkdir bin"
        "\n cd bin"
        "\n cmake .."
        "\n")
endif()

cmake_minimum_required(VERSION 2.8)
if(APPLE)
    if(ARCH)
        set(CMAKE_OSX_ARCHITECTURES ${ARCH})
    else()
        set(CMAKE_OSX_ARCHITECTURES "x86_64;i386")
    endif()
    if(MACOSX_SDK)
        set(CMAKE_OSX_SYSROOT ${MACOSX_SDK})
    endif()
elseif(WIN32)
    if(DEFINED ENV{MINGW_PREFIX})
        if(DEFINED ENV{PKG_CONFIG_PATH})
            set(pkg_path)
            foreach(_path $ENV{PKG_CONFIG_PATH})
                string(REGEX REPLACE "^[A-Z]:/" "\\\\" _apath ${_path})
                set(pkg_path "${pkg_path}:${_apath}")
            endforeach()
            string(REGEX REPLACE "^:" "" _xpath ${pkg_path})
            set(ENV{PKG_CONFIG_PATH} ${_xpath})
        else()
            set(_xpath "$ENV{MINGW_PREFIX}/lib/pkgconfig")
            string(REGEX REPLACE "^[A-Z]:/" "\\\\" _ypath ${_xpath})
            set(ENV{PKG_CONFIG_PATH} ${_ypath})
        endif()
        message(STATUS "pkg-config path: $ENV{PKG_CONFIG_PATH}")
    endif()
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()


if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

if(UNIX)
    if(APPLE)
        set(CMAKE_MACOSX_RPATH FALSE)
        # @rpath on macosx
        # set(CMAKE_SKIP_RPATH True)
        # set(CMAKE_OSX_DEPLOYMENT_TARGET "10.6")
        # set(CMAKE_SKIP_BUILD_RPATH FALSE)
        # set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
        # set(CMAKE_INSTALL_RPATH "@loader_path")
        # set(CMAKE_INSTALL_RPATH_USE_LINK_PATH FALSE)
     else()
        set(CMAKE_SKIP_BUILD_RPATH FALSE)
        set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
        set(CMAKE_INSTALL_RPATH "\$ORIGIN")
     endif()
endif()

project(puredata)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99")
enable_language(CXX C)
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
include(version)
include(build_options)
include(install_paths)
find_package(LeapMotion)
include(PdPlatform)
include(PdExternal)
find_package(Gettext)
find_package(LibUSB)
include(config)
include(sound)
include(cmake_uninstall)

# libsndfile backend
if(WITH_LIBSNDFILE)
    include(FindLibSndFile)
endif()

# FFTW3
option(WITH_FFTW "Use fftw3 library (http://www.fftw.org/)" ON)
if(WITH_FFTW)
    include(FindFFTW)
    if(NOT FFTW_FOUND)
        set(WITH_FFTW OFF CACHE BOOL "" FORCE)
    endif()
endif()

add_subdirectory(portmidi)
add_subdirectory(src)
add_subdirectory(tcl)
add_subdirectory(extra)
add_subdirectory(doc)

if(GETTEXT_FOUND)
    add_subdirectory(po)
endif()

if(ENABLE_TESTS)
    enable_testing(true)
endif()

add_subdirectory(ceammc)

include(summary)
include(package)

if(APPLE)
    add_custom_target(mac_deploy SOURCES mac/cmake-build-mac.sh.in mac/codesign.sh mac/dmg.sh)
endif()

add_custom_target(puredata_changelog SOURCES CHANGELOG.md)

file(GLOB _DEB_FILES "debian/*")
add_custom_target(deb_files SOURCES ${_DEB_FILES})

add_custom_target(ci_files SOURCES .travis.yml appveyor.yml)
