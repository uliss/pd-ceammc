include(library_pddoc.cmake)
include(CheckIncludeFileCXX)


add_subdirectory(SoundTouch)
add_subdirectory(aubio)
add_subdirectory(comport)
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

add_subdirectory(midifile)
add_subdirectory(muparser)
add_subdirectory(serial)
add_subdirectory(sf2)
add_subdirectory(soxr)
add_subdirectory(stk)
add_subdirectory(subprocess)
add_subdirectory(reflex)
add_subdirectory(glitch)

if(WITH_FAUST)
    add_subdirectory(faust)
endif()

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(CMAKE_BUILD_TYPE Release)
add_subdirectory(fmt)
set_target_properties(fmt PROPERTIES DEBUG_POSTFIX "")

option(RE2_BUILD_TESTING "" OFF)
set(RE2_BUILD_TESTING OFF CACHE BOOL "" FORCE)
add_subdirectory(re2)

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
