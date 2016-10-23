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
set(CMAKE_OSX_ARCHITECTURES "x86_64;i386")
set(CMAKE_OSX_SYSROOT macosx10.11)
set(CMAKE_OSX_DEPLOYMENT_TARGET "10.6")
set(CMAKE_SKIP_RPATH True)
set(CMAKE_MACOSX_RPATH 0)
project(puredata)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99")
enable_language(CXX C)
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
include(version)
include(install_paths)
include(PdPlatform)
find_package(Gettext)
include(config)
include(sound)

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

add_subdirectory(ceammc)

include(summary)
include(package)
