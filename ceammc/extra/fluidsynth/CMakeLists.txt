# FluidSynth package version
set(FLUIDSYNTH_VERSION_MAJOR 2)
set(FLUIDSYNTH_VERSION_MINOR 3)
set(FLUIDSYNTH_VERSION_MICRO 2)
set(VERSION "${FLUIDSYNTH_VERSION_MAJOR}.${FLUIDSYNTH_VERSION_MINOR}.${FLUIDSYNTH_VERSION_MICRO}")
set(FLUIDSYNTH_VERSION "\"${VERSION}\"")

# libfluidsynth - Library version
# *** NOTICE ***
# Update library version upon each release (follow these steps in order)
# if any source code changes: REVISION++
# if any interfaces added/removed/changed: REVISION=0
# if any interfaces removed/changed (compatibility broken): CURRENT++
# if any interfaces have been added: AGE++
# if any interfaces have been removed/changed (compatibility broken): AGE=0
# This is not exactly the same algorithm as the libtool one, but the results are the same.
set(LIB_VERSION_CURRENT 2)
set(LIB_VERSION_AGE 3)
set(LIB_VERSION_REVISION 2)
set(LIB_VERSION_INFO "${LIB_VERSION_CURRENT}.${LIB_VERSION_AGE}.${LIB_VERSION_REVISION}")

include(TestBigEndian)
test_big_endian(WORDS_BIGENDIAN)

if(WIN32)
    include(CheckIncludeFiles)
    check_include_file(windows.h HAVE_WINDOWS_H)
    check_include_file(io.h HAVE_IO_H)
    check_include_files("windows.h;mmsystem.h" HAVE_MMSYSTEM_H)

    set(WINDOWS_SUPPORT ${HAVE_WINDOWS_H})
    set(WINDOWS_LIBS "dsound;winmm;ws2_32")

    set(LIBFLUID_CPPFLAGS "-DFLUIDSYNTH_DLL_EXPORTS")
    set(FLUID_CPPFLAGS "-DFLUIDSYNTH_NOT_A_DLL")

    # MinGW compiler (a Windows GCC port)
    if(MINGW)
        set(MINGW32 1)
        add_definitions(-mms-bitfields)
    endif (MINGW)
else(WIN32)
# Check PThreads, but not in Windows
    find_package(Threads REQUIRED)
    set(HAVE_LIBPTHREAD ${Threads_FOUND})
    set(LIBFLUID_LIBS "m" ${CMAKE_THREAD_LIBS_INIT})
endif(WIN32)

set(WITH_FLOAT 1)

# General configuration file
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.cmake ${CMAKE_CURRENT_BINARY_DIR}/config.h)
add_definitions(-include ${PROJECT_BINARY_DIR}/config.h)

include_directories (
    ${GLIB_INCLUDES}
    ${CMAKE_CURRENT_BINARY_DIR}
    fluidsynth/src
    fluidsynth/src/drivers
    fluidsynth/src/synth
    fluidsynth/src/rvoice
    fluidsynth/src/midi
    fluidsynth/src/utils
    fluidsynth/src/sfloader
    fluidsynth/src/bindings
    fluidsynth/include
    ${PTHREADS_INCLUDE_DIR}
)

set(config_SOURCES ${CMAKE_CURRENT_BINARY_DIR}/config.h)

set(libfluidsynth_SOURCES
    fluidsynth/src/utils/fluid_conv.c
    fluidsynth/src/utils/fluid_conv.h
    fluidsynth/src/utils/fluid_conv_tables.h
    fluidsynth/src/utils/fluid_hash.c
    fluidsynth/src/utils/fluid_hash.h
    fluidsynth/src/utils/fluid_list.c
    fluidsynth/src/utils/fluid_list.h
    fluidsynth/src/utils/fluid_ringbuffer.c
    fluidsynth/src/utils/fluid_ringbuffer.h
    fluidsynth/src/utils/fluid_settings.c
    fluidsynth/src/utils/fluid_settings.h
    fluidsynth/src/utils/fluid_sys.c
    fluidsynth/src/utils/fluid_sys.h
    fluidsynth/src/utils/fluidsynth_priv.h
    fluidsynth/src/sfloader/fluid_defsfont.c
    fluidsynth/src/sfloader/fluid_defsfont.h
    fluidsynth/src/sfloader/fluid_sfont.h
    fluidsynth/src/sfloader/fluid_sfont.c
    fluidsynth/src/sfloader/fluid_sffile.c
    fluidsynth/src/sfloader/fluid_sffile.h
    fluidsynth/src/sfloader/fluid_samplecache.c
    fluidsynth/src/sfloader/fluid_samplecache.h
    fluidsynth/src/rvoice/fluid_adsr_env.c
    fluidsynth/src/rvoice/fluid_adsr_env.h
    fluidsynth/src/rvoice/fluid_chorus.c
    fluidsynth/src/rvoice/fluid_chorus.h
    fluidsynth/src/rvoice/fluid_iir_filter.c
    fluidsynth/src/rvoice/fluid_iir_filter.h
    fluidsynth/src/rvoice/fluid_lfo.c
    fluidsynth/src/rvoice/fluid_lfo.h
    fluidsynth/src/rvoice/fluid_rvoice.h
    fluidsynth/src/rvoice/fluid_rvoice.c
    fluidsynth/src/rvoice/fluid_rvoice_dsp.c
    fluidsynth/src/rvoice/fluid_rvoice_event.h
    fluidsynth/src/rvoice/fluid_rvoice_event.c
    fluidsynth/src/rvoice/fluid_rvoice_mixer.h
    fluidsynth/src/rvoice/fluid_rvoice_mixer.c
    fluidsynth/src/rvoice/fluid_phase.h
    fluidsynth/src/rvoice/fluid_rev.c
    fluidsynth/src/rvoice/fluid_rev.h
    fluidsynth/src/synth/fluid_chan.c
    fluidsynth/src/synth/fluid_chan.h
    fluidsynth/src/synth/fluid_event.c
    fluidsynth/src/synth/fluid_gen.c
    fluidsynth/src/synth/fluid_gen.h
    fluidsynth/src/synth/fluid_mod.c
    fluidsynth/src/synth/fluid_mod.h
    fluidsynth/src/synth/fluid_synth.c
    fluidsynth/src/synth/fluid_synth.h
    fluidsynth/src/synth/fluid_synth_monopoly.c
    fluidsynth/src/synth/fluid_tuning.c
    fluidsynth/src/synth/fluid_tuning.h
    fluidsynth/src/synth/fluid_voice.c
    fluidsynth/src/synth/fluid_voice.h
    fluidsynth/src/midi/fluid_midi.c
    fluidsynth/src/midi/fluid_midi.h
    fluidsynth/src/midi/fluid_midi_router.c
    fluidsynth/src/midi/fluid_midi_router.h
    fluidsynth/src/midi/fluid_seqbind.c
    fluidsynth/src/midi/fluid_seq.c
    fluidsynth/src/drivers/fluid_adriver.c
    fluidsynth/src/drivers/fluid_adriver.h
    fluidsynth/src/drivers/fluid_mdriver.c
    fluidsynth/src/drivers/fluid_mdriver.h
    fluidsynth/src/drivers/fluid_aufile.c
    fluidsynth/src/bindings/fluid_cmd.c
    fluidsynth/src/bindings/fluid_cmd.h
    fluidsynth/src/bindings/fluid_filerenderer.c
)

if(WIN32 OR MINGW)
    list(APPEND libfluidsynth_SOURCES fluidsynth/src/drivers/fluid_dsound.c)
    list(APPEND libfluidsynth_SOURCES fluidsynth/src/drivers/fluid_winmidi.c)
endif()

set(public_HEADERS
    fluidsynth/include/fluidsynth/audio.h
    fluidsynth/include/fluidsynth/event.h
    fluidsynth/include/fluidsynth/gen.h
    fluidsynth/include/fluidsynth/ladspa.h
    fluidsynth/include/fluidsynth/log.h
    fluidsynth/include/fluidsynth/midi.h
    fluidsynth/include/fluidsynth/misc.h
    fluidsynth/include/fluidsynth/mod.h
    fluidsynth/include/fluidsynth/seq.h
    fluidsynth/include/fluidsynth/seqbind.h
    fluidsynth/include/fluidsynth/settings.h
    fluidsynth/include/fluidsynth/sfont.h
    fluidsynth/include/fluidsynth/shell.h
    fluidsynth/include/fluidsynth/synth.h
    fluidsynth/include/fluidsynth/types.h
    fluidsynth/include/fluidsynth/voice.h
    ${CMAKE_CURRENT_BINARY_DIR}/fluidsynth/version.h
)

set(public_main_HEADER ${CMAKE_CURRENT_BINARY_DIR}/fluidsynth.h)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/fluidsynth/include/fluidsynth/version.h.in
                 ${CMAKE_CURRENT_BINARY_DIR}/fluidsynth/version.h)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/fluidsynth/include/fluidsynth.cmake
                 ${public_main_HEADER})

# install soundfonts
install(
    FILES
        fluidsynth/sf2/VintageDreamsWaves-v2.sf2
    DESTINATION
        ${CEAMMC_INSTALL_PATH}/sf2
    PERMISSIONS OWNER_READ GROUP_READ WORLD_READ)
install(
    FILES
        fluidsynth/sf2/COPYRIGHT.txt
    DESTINATION
        ${CEAMMC_INSTALL_PATH}/sf2
    PERMISSIONS OWNER_READ GROUP_READ WORLD_READ)

add_library(fluidsynth STATIC
    ${config_SOURCES}
    ${fluid_windows_SOURCES}
    ${libfluidsynth_SOURCES}
    ${public_HEADERS}
    ${public_main_HEADER}
    ${VersionFilesOutputVariable}
)

if(LIBFLUID_CPPFLAGS)
    set_target_properties (fluidsynth PROPERTIES COMPILE_FLAGS ${LIBFLUID_CPPFLAGS})
endif()

if(APPLE)
    target_link_directories(fluidsynth PUBLIC ${GLIB_LIBRARY_DIRS})
endif()

target_link_libraries(fluidsynth
    ${GLIB_LIBRARIES}
    ${WINDOWS_LIBS}
    ${LIBFLUID_LIBS}
)

add_executable(fluidsynth_make_table
    fluidsynth/src/gentables/gen_conv.c
    fluidsynth/src/gentables/gen_rvoice_dsp.c
    fluidsynth/src/gentables/make_tables.c
    fluidsynth/src/gentables/make_tables.h)

add_custom_command(OUTPUT fluid_conv_tables.c
    DEPENDS fluidsynth_make_table
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/fluidsynth_make_table "${CMAKE_CURRENT_BINARY_DIR}/"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

add_custom_target(fluid_gen_tables DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/fluid_conv_tables.c")
target_link_libraries(fluidsynth_make_table PRIVATE m)
add_dependencies(fluidsynth fluid_gen_tables)

if(UNIX)
    set_target_properties(fluidsynth PROPERTIES COMPILE_FLAGS "-fPIC")
endif()
