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

option(WITH_DUMMY "Build dummy audio and midi modules" OFF)
cmake_dependent_option(WITH_DUMMY_AUDIO "Build dummy audio module" ON "WITH_DUMMY" OFF)
cmake_dependent_option(WITH_DUMMY_MIDI "Build dummy midi module" ON "WITH_DUMMY" OFF)

# ALSA
if(WITH_ALSA_AUDIO OR WITH_ALSA_MIDI)
    include(FindALSA)
    if(NOT ALSA_FOUND)
        set(WITH_ALSA OFF CACHE BOOL "" FORCE)
        set(WITH_ALSA_AUDIO OFF CACHE BOOL "" FORCE)
        set(WITH_ALSA_MIDI OFF CACHE BOOL "" FORCE)
    else()
        add_definitions(-DUSEAPI_ALSA)
    endif()
endif()

# JACK
if(WITH_JACK)
    include(FindJack)
    if(NOT JACK_FOUND)
        set(WITH_JACK OFF CACHE BOOL "" FORCE)
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

if(NOT WITH_ALSA_AUDIO AND
        NOT WITH_JACK AND
        NOT WITH_OSS_AUDIO AND
        NOT WITH_PORTAUDIO AND
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
