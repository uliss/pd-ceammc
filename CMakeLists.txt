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
project(puredata)

add_definitions(-std=c99)
enable_language(CXX C)
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
include(PdPlatform)
include(CheckIncludeFiles)
include(CheckFunctionExists)
include(FindALSA)
include(FindPortAudio)
include(FindLibDL)

option(SOUND_DUMMY "Build dummy sound module" OFF)

# FFTW3
option(WITH_FFTW "Use fftw3 library (http://www.fftw.org/)" ON)
if(WITH_FFTW)
    include(FindFFTW)
    if(!FFTW_FOUND)
        unset(WITH_FFTW)
    endif()
endif()

# JACK
option(WITH_JACK "Use jackd" ON)
if(WITH_JACK)
    include(FindJack)
    if(!JACK_FOUND)
        unset(WITH_JACK)
    endif()
endif()

check_include_files(unistd.h HAVE_UNISTD_H)
check_include_files(alloca.h HAVE_ALLOCA_H)
check_include_files(bstring.h HAVE_BSTRING_H)
check_include_files(sys/soundfile.h HAVE_SYS_SOUNDCARD_H)
check_function_exists(nanosleep HAVE_NANOSLEEP)

if(LIBDL_FOUND)
    set(HAVE_LIBDL 1)
endif()



CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/config.h.in ${PROJECT_BINARY_DIR}/config.h)
add_definitions(-include ${PROJECT_BINARY_DIR}/config.h)

add_subdirectory(src)
add_subdirectory(tcl)
add_subdirectory(extra)
