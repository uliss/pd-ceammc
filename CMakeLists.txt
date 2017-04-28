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
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

if(APPLE)
    set(CMAKE_MACOSX_RPATH FALSE)
    # @rpath on macosx
    # set(CMAKE_SKIP_RPATH True)
    # set(CMAKE_OSX_DEPLOYMENT_TARGET "10.6")
    # set(CMAKE_SKIP_BUILD_RPATH FALSE)
    # set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
    # set(CMAKE_INSTALL_RPATH "@loader_path")
    # set(CMAKE_INSTALL_RPATH_USE_LINK_PATH FALSE)
endif()

project(puredata)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99")
enable_language(CXX C)
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
include(version)
include(build_options)
include(install_paths)
include(PdPlatform)
find_package(Gettext)
include(config)
include(sound)
include(cmake_uninstall)

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

enable_testing(true)
add_subdirectory(ceammc)

include(summary)
include(package)

if(APPLE)
    add_custom_target(mac_deploy SOURCES mac/cmake-build-mac.sh.in mac/codesign.sh mac/dmg.sh)
endif()
