include(library_pddoc.cmake)
include(CheckIncludeFileCXX)


add_subdirectory(SoundTouch)
add_subdirectory(aubio)
add_subdirectory(comport)
add_subdirectory(fftconv)
add_subdirectory(helmholtz)
add_subdirectory(import)
add_subdirectory(libdir)
add_subdirectory(airwindows)

if(WITH_EXT_AUTOTUNE)
    add_subdirectory(autotune)
endif()

if(WITH_EXT_FLEXT)
    add_subdirectory(flext)
endif()

if(WITH_EXT_FFTEASE)
    add_subdirectory(fftease)
endif()

if(WITH_EXT_LYONPOTPOURRI)
    add_subdirectory(lyonpotpourri)
endif()

add_subdirectory(glitch)
add_subdirectory(midifile)
add_subdirectory(muparser)
add_subdirectory(reflex)
add_subdirectory(serial)
add_subdirectory(sf2)
add_subdirectory(soxr)
add_subdirectory(stk)
add_subdirectory(subprocess)
add_subdirectory(httplib)

if(WITH_FAUST)
    add_subdirectory(faust)
endif()

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(CMAKE_BUILD_TYPE Release)
add_subdirectory(fmt)
set_target_properties(fmt PROPERTIES DEBUG_POSTFIX "")

option(RE2_BUILD_TESTING "" OFF)
set(RE2_BUILD_TESTING OFF CACHE BOOL "" FORCE)
add_subdirectory(re2 EXCLUDE_FROM_ALL)

option(HOA_BUILD_TESTS "" OFF)
set(HOA_BUILD_TESTS OFF CACHE BOOL "" FORCE)
add_subdirectory(HoaLibrary-Light)

if(UNIX)
    set_target_properties(re2 PROPERTIES COMPILE_FLAGS "-fPIC")
    set_target_properties(fmt PROPERTIES COMPILE_FLAGS "-fPIC")
endif()

if(WITH_EXPERIMENTAL)
    add_subdirectory(libusb)
endif()

if(WITH_FLUIDSYNTH)
    add_subdirectory(fluidsynth)
endif()

if(WITH_TTS_FLITE)
    add_subdirectory(speech)
endif()

set(EXTRA_COMPLETION "extra_objects.txt")
add_custom_target(_ceammc_extra_misc SOURCES ${EXTRA_COMPLETION})

if(WITH_SFIZZ)
    cmake_minimum_required(VERSION 3.14)

    include(FetchContent)
    set(FETCHCONTENT_QUIET FALSE)
    set(SFIZZ_AU OFF CACHE INTERNAL "")
    set(SFIZZ_LV2 OFF CACHE INTERNAL "")
    set(SFIZZ_LV2_UI OFF CACHE INTERNAL "")
    set(SFIZZ_VST OFF CACHE INTERNAL "")
    set(SFIZZ_JACK OFF CACHE INTERNAL "")
    set(SFIZZ_RENDER OFF CACHE INTERNAL "")

    if(WIN32)
        set(SFIZZ_SHARED ON CACHE INTERNAL "")
    else()
        set(SFIZZ_SHARED OFF CACHE INTERNAL "")
    endif()

    set(SFIZZ_USE_SNDFILE OFF CACHE INTERNAL "")

    FetchContent_Declare(
      sfizz
      URL      https://github.com/sfztools/sfizz/releases/download/1.1.1/sfizz-1.1.1.tar.gz
      URL_HASH MD5=cc4770c287d52da517a7ae25fd826a3e
    )
    FetchContent_GetProperties(sfizz)
    if(NOT sfizz_POPULATED)
      FetchContent_Populate(sfizz)
      add_subdirectory(${sfizz_SOURCE_DIR} ${sfizz_BINARY_DIR} EXCLUDE_FROM_ALL)
    endif()
endif()

add_library(readerwriterqueue INTERFACE)
target_include_directories(readerwriterqueue INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/readerwriterqueue)
