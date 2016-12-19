include(CMakeDependentOption)
include(CheckIncludeFiles)

option(WITH_ALSA "Use ALSA sound API" ON)
cmake_dependent_option(WITH_ALSA_AUDIO "Use OSS audio API" ON "WITH_ALSA" OFF)
cmake_dependent_option(WITH_ALSA_MIDI "Use OSS midi API" ON "WITH_ALSA" OFF)
option(WITH_JACK "Use Jack sound API" ON)

option(WITH_OSS "Use OSS sound API" ON)
cmake_dependent_option(WITH_OSS_AUDIO "Use OSS audio API" ON "WITH_OSS" OFF)
cmake_dependent_option(WITH_OSS_MIDI "Use OSS midi API" ON "WITH_OSS" OFF)

option(WITH_PORTAUDIO "Use PortAudio sound API" ON)
option(WITH_PORTMIDI "Use PortMidi sound API" ON)

if(MSYS OR WIN32)
    option(WITH_MMIO "Use MMIO sound API" ON)
endif()

option(WITH_DUMMY "Build dummy audio and midi modules" OFF)
option(WITH_DUMMY_AUDIO "Build dummy audio module" OFF)
option(WITH_DUMMY_MIDI "Build dummy midi module" OFF)
if(WITH_DUMMY)
    set(WITH_DUMMY_AUDIO ON CACHE BOOL "" FORCE)
    set(WITH_DUMMY_MIDI ON CACHE BOOL "" FORCE)
endif()


# ALSA
if(WITH_ALSA_AUDIO OR WITH_ALSA_MIDI)
    include(FindALSA)
    if(ALSA_FOUND)
        add_definitions(-DUSEAPI_ALSA)
        include_directories(${ALSA_INCLUDE_DIR})
    else()
        set(WITH_ALSA OFF CACHE BOOL "" FORCE)
        set(WITH_ALSA_AUDIO OFF CACHE BOOL "" FORCE)
        set(WITH_ALSA_MIDI OFF CACHE BOOL "" FORCE)
    endif()
endif()

# JACK
if(WITH_JACK)
    include(FindJack)
    if(JACK_FOUND)
        add_definitions(-DUSEAPI_JACK -DJACK_XRUN)
        include_directories(${JACK_INCLUDE_DIRECTORIES})
    else()
        set(WITH_JACK OFF CACHE BOOL "" FORCE)
    endif()
endif()

# OSS
if(WITH_OSS_AUDIO OR WITH_OSS_MIDI)
    check_include_files(sys/soundcard.h HAVE_SYS_SOUNDCARD_H)
    if(HAVE_SYS_SOUNDCARD_H)
        add_definitions(-DUSEAPI_OSS)
    else()
        set(WITH_OSS OFF CACHE BOOL "" FORCE)
        set(WITH_OSS_AUDIO OFF CACHE BOOL "" FORCE)
        set(WITH_OSS_MIDI OFF CACHE BOOL "" FORCE)
    endif()
endif()

# PORTAUDIO
if(WITH_PORTAUDIO)
    find_package(PortAudio REQUIRED)
    if(PORTAUDIO_FOUND)
        add_definitions(-DUSEAPI_PORTAUDIO)
        include_directories(${PORTAUDIO_INCLUDE_DIRS})
    else()
        set(WITH_PORTAUDIO OFF CACHE BOOL "" FORCE)
    endif()
endif()

# Dummy
if(WITH_DUMMY_AUDIO OR WITH_DUMMY_MIDI)
    add_definitions(-DUSEAPI_DUMMY)
endif()

if(NOT WITH_ALSA_AUDIO AND
        NOT WITH_JACK AND
        NOT WITH_OSS_AUDIO AND
        NOT WITH_PORTAUDIO AND
        NOT WITH_MMIO AND
        NOT WITH_DUMMY_AUDIO)
    message(FATAL_ERROR "At least one sound module required! See `cmake -L` for available config options.
        You can build with dummy audio with cmake -DWITH_DUMMY_AUDIO=ON option.")
endif()

if(NOT WITH_ALSA_MIDI AND
        NOT WITH_OSS_MIDI AND
        NOT WITH_PORTMIDI AND
        NOT WITH_DUMMY_MIDI)
    message(FATAL_ERROR "At least one midi module required! See `cmake -L` for available config options.
        You can build with dummy midi with cmake -DWITH_DUMMY_MIDI=ON option.")
endif()
