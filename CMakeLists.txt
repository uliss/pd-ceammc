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

add_definitions(-std=c99)
enable_language(CXX C)
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
include(CheckIncludeFiles)
include(CheckFunctionExists)
include(CMakeDependentOption)
include(PdPlatform)
include(FindLibDL)
find_package(Gettext)

option(WITH_ALSA "Use ALSA sound API" ON)
cmake_dependent_option(WITH_ALSA_AUDIO "Use OSS audio API" ON "WITH_ALSA" OFF)
cmake_dependent_option(WITH_ALSA_MIDI "Use OSS midi API" ON "WITH_ALSA" OFF)
option(WITH_JACK "Use Jack sound API" ON)

option(WITH_OSS "Use OSS sound API" ON)
cmake_dependent_option(WITH_OSS_AUDIO "Use OSS audio API" ON "WITH_OSS" OFF)
cmake_dependent_option(WITH_OSS_MIDI "Use OSS midi API" ON "WITH_OSS" OFF)

option(WITH_PORTAUDIO "Use PortAudio sound API" ON)
option(WITH_PORTMIDI "Use PortMidi sound API" ON)

option(WITH_DUMMY "Build dummy audio and midi modules" OFF)
cmake_dependent_option(WITH_DUMMY_AUDIO "Build dummy audio module" ON "WITH_DUMMY" OFF)
cmake_dependent_option(WITH_DUMMY_MIDI "Build dummy midi module" ON "WITH_DUMMY" OFF)

# ALSA
if(WITH_ALSA_AUDIO OR WITH_ALSA_MIDI)
    include(FindALSA)
    if(NOT ALSA_FOUND)
        set(WITH_ALSA OFF)
        set(WITH_ALSA_AUDIO OFF)
        set(WITH_ALSA_MIDI OFF)
    else()
        add_definitions(-DUSEAPI_ALSA)
    endif()
endif()

# JACK
if(WITH_JACK)
    include(FindJack)
    if(NOT JACK_FOUND)
        set(WITH_JACK OFF)
    else()
         add_definitions(-DUSEAPI_JACK -DJACK_XRUN)
    endif()
endif()

# OSS
if(WITH_OSS_AUDIO OR WITH_OSS_MIDI)
    check_include_files(sys/soundcard.h HAVE_SYS_SOUNDCARD_H)
    if(NOT HAVE_SYS_SOUNDCARD_H)
        set(WITH_OSS OFF CACHE BOOL "" FORCE)
        set(WITH_OSS_AUDIO OFF CACHE BOOL "" FORCE)
        set(WITH_OSS_MIDI OFF CACHE BOOL "" FORCE)
    else()
        add_definitions(-DUSEAPI_OSS)
    endif()
endif()

# PORTAUDIO
if(WITH_PORTAUDIO)
    include(FindPortAudio)
    if(PORTAUDIO_FOUND)
        add_definitions(-DUSEAPI_PORTAUDIO)
    else()
        set(WITH_PORTAUDIO OFF CACHE BOOL "" FORCE)
    endif()
endif()

# PORTMIDI
if(WITH_PORTMIDI)
    include(FindPortMidi)
    if(NOT PORTMIDI_FOUND)
        set(WITH_PORTMIDI OFF CACHE BOOL "" FORCE)
    endif()
endif()

# Dummy
if(WITH_DUMMY_AUDIO OR WITH_DUMMY_MIDI)
    add_definitions(-DUSEAPI_DUMMY)
endif()

check_include_files(unistd.h HAVE_UNISTD_H)
check_include_files(alloca.h HAVE_ALLOCA_H)
check_include_files(bstring.h HAVE_BSTRING_H)
check_function_exists(nanosleep HAVE_NANOSLEEP)

if(LIBDL_FOUND)
    set(HAVE_LIBDL 1)
endif()

# FFTW3
option(WITH_FFTW "Use fftw3 library (http://www.fftw.org/)" ON)
if(WITH_FFTW)
    include(FindFFTW)
    if(!FFTW_FOUND)
        unset(WITH_FFTW)
    endif()
endif()


configure_file(${PROJECT_SOURCE_DIR}/config.h.in ${PROJECT_BINARY_DIR}/config.h)
add_definitions(-include ${PROJECT_BINARY_DIR}/config.h)

add_subdirectory(src)
add_subdirectory(tcl)
add_subdirectory(extra)
add_subdirectory(doc)

if(GETTEXT_FOUND)
    add_subdirectory(po)
endif()


include(summary)
