#add_subdirectory(fluidsynth)

# FluidSynth - A Software Synthesizer
#
# Copyright (C) 2003-2010 Peter Hanappe and others.
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public License
# as published by the Free Software Foundation; either version 2.1 of
# the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
# 02111-1307, USA

# CMake based build system. Pedro Lopez-Cabanillas <plcl@users.sf.net>


# FluidSynth package version
set ( FLUIDSYNTH_VERSION_MAJOR 2 )
set ( FLUIDSYNTH_VERSION_MINOR 0 )
set ( FLUIDSYNTH_VERSION_MICRO 0 )
set ( VERSION "${FLUIDSYNTH_VERSION_MAJOR}.${FLUIDSYNTH_VERSION_MINOR}.${FLUIDSYNTH_VERSION_MICRO}" )
set ( FLUIDSYNTH_VERSION "\"${VERSION}\"" )

# libfluidsynth - Library version
# *** NOTICE ***
# Update library version upon each release (follow these steps in order)
# if any source code changes: REVISION++
# if any interfaces added/removed/changed: REVISION=0
# if any interfaces removed/changed (compatibility broken): CURRENT++
# if any interfaces have been added: AGE++
# if any interfaces have been removed/changed (compatibility broken): AGE=0
# This is not exactly the same algorithm as the libtool one, but the results are the same.
set ( LIB_VERSION_CURRENT 2 )
set ( LIB_VERSION_AGE 0 )
set ( LIB_VERSION_REVISION 0 )
set ( LIB_VERSION_INFO
      "${LIB_VERSION_CURRENT}.${LIB_VERSION_AGE}.${LIB_VERSION_REVISION}" )

#include ( CheckSTDC )
include ( CheckIncludeFile )
include ( CheckFunctionExists )
check_include_file ( string.h HAVE_STRING_H )
check_include_file ( stdlib.h HAVE_STDLIB_H )
check_include_file ( stdio.h HAVE_STDIO_H )
check_include_file ( math.h HAVE_MATH_H )
check_include_file ( errno.h HAVE_ERRNO_H )
check_include_file ( stdarg.h HAVE_STDARG_H )
check_include_file ( unistd.h HAVE_UNISTD_H )
check_include_file ( memory.h HAVE_MEMORY_H )
check_include_file ( sys/mman.h HAVE_SYS_MMAN_H )
check_include_file ( sys/types.h HAVE_SYS_TYPES_H )
check_include_file ( sys/time.h HAVE_SYS_TIME_H )
check_include_file ( sys/stat.h HAVE_SYS_STAT_H )
check_include_file ( sys/ioctl.h HAVE_SYS_IOCTL_H )
check_include_file ( fcntl.h HAVE_FCNTL_H )
check_include_file ( sys/socket.h HAVE_SYS_SOCKET_H )
check_include_file ( netinet/in.h HAVE_NETINET_IN_H )
check_include_file ( netinet/tcp.h HAVE_NETINET_TCP_H )
check_include_file ( arpa/inet.h HAVE_ARPA_INET_H )
check_include_file ( limits.h  HAVE_LIMITS_H )
check_include_file ( pthread.h HAVE_PTHREAD_H )
check_include_file ( signal.h HAVE_SIGNAL_H )
check_include_file ( getopt.h HAVE_GETOPT_H )
check_include_file ( stdint.h HAVE_STDINT_H )
#include ( TestInline )
#include ( TestVLA )
include ( TestBigEndian )
test_big_endian ( WORDS_BIGENDIAN )

if ( WIN32 )
  include ( CheckIncludeFiles )
  check_include_file ( windows.h HAVE_WINDOWS_H )
  check_include_file ( io.h HAVE_IO_H )
  check_include_file ( dsound.h HAVE_DSOUND_H )
  check_include_files ( "windows.h;mmsystem.h" HAVE_MMSYSTEM_H )
  set ( WINDOWS_SUPPORT ${HAVE_WINDOWS_H} )
  set ( WINDOWS_LIBS "dsound;winmm;ws2_32" )
  set ( LIBFLUID_CPPFLAGS "-DFLUIDSYNTH_DLL_EXPORTS" )
  set ( FLUID_CPPFLAGS "-DFLUIDSYNTH_NOT_A_DLL" )
  if (NOT MSVC)
    set ( CMAKE_DEBUG_POSTFIX "_debug" )
  endif()
  # MinGW compiler (a Windows GCC port)
  if ( MINGW )
    set ( MINGW32 1 )
    add_definitions ( -mms-bitfields )
  endif  ( MINGW )
else ( WIN32 )
# Check PThreads, but not in Windows
  find_package ( Threads REQUIRED )
  set ( HAVE_LIBPTHREAD ${Threads_FOUND} )
  set ( LIBFLUID_LIBS "m" ${CMAKE_THREAD_LIBS_INIT} )
endif ( WIN32 )

set ( WITH_FLOAT 1 )

# General configuration file
configure_file ( ${CMAKE_CURRENT_SOURCE_DIR}/fluidsynth/src/config.cmake
                 ${CMAKE_CURRENT_BINARY_DIR}/config.h )

include_directories (
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

include_directories (
    SYSTEM
    ${GLIB_INCLUDEDIR}
    ${GLIB_INCLUDE_DIRS}
)

#if ( WINDOWS_SUPPORT )
#  set ( fluid_windows_SOURCES drivers/fluid_dsound.c drivers/fluid_winmidi.c )
#endif ()

set ( config_SOURCES ${CMAKE_CURRENT_BINARY_DIR}/config.h )

set ( libfluidsynth_SOURCES
    fluidsynth/src/utils/fluid_conv.c
    fluidsynth/src/utils/fluid_conv.h
    fluidsynth/src/utils/fluid_hash.c
    fluidsynth/src/utils/fluid_hash.h
    fluidsynth/src/utils/fluid_list.c
    fluidsynth/src/utils/fluid_list.h
    fluidsynth/src/utils/fluid_ringbuffer.c
    fluidsynth/src/utils/fluid_ringbuffer.h
    fluidsynth/src/utils/fluid_settings.c
    fluidsynth/src/utils/fluid_settings.h
    fluidsynth/src/utils/fluidsynth_priv.h
    fluidsynth/src/utils/fluid_sys.c
    fluidsynth/src/utils/fluid_sys.h
    fluidsynth/src/sfloader/fluid_defsfont.c
    fluidsynth/src/sfloader/fluid_defsfont.h
    fluidsynth/src/sfloader/fluid_ramsfont.c
    fluidsynth/src/sfloader/fluid_ramsfont.h
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
    fluidsynth/src/synth/fluid_event_priv.h
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

set ( public_HEADERS
    fluidsynth/include/fluidsynth/audio.h
    fluidsynth/include/fluidsynth/event.h
    fluidsynth/include/fluidsynth/gen.h
    fluidsynth/include/fluidsynth/ladspa.h
    fluidsynth/include/fluidsynth/log.h
    fluidsynth/include/fluidsynth/midi.h
    fluidsynth/include/fluidsynth/misc.h
    fluidsynth/include/fluidsynth/mod.h
    fluidsynth/include/fluidsynth/ramsfont.h
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

set ( public_main_HEADER
    ${CMAKE_CURRENT_BINARY_DIR}/fluidsynth.h
)

configure_file ( ${CMAKE_CURRENT_SOURCE_DIR}/fluidsynth/include/fluidsynth/version.h.in
                 ${CMAKE_CURRENT_BINARY_DIR}/fluidsynth/version.h )
configure_file ( ${CMAKE_CURRENT_SOURCE_DIR}/fluidsynth/include/fluidsynth.cmake
                 ${public_main_HEADER} )

if ( WIN32 AND NOT MINGW )
include(generate_product_version)
generate_product_version(
    VersionFilesOutputVariable
    NAME "Fluidsynth"
    BUNDLE "Fluidsynth"
    VERSION_MAJOR ${FLUIDSYNTH_VERSION_MAJOR}
    VERSION_MINOR ${FLUIDSYNTH_VERSION_MINOR}
    VERSION_PATCH ${FLUIDSYNTH_VERSION_MICRO}
    VERSION_REVISION 0
    COMMENTS "Fluidsynth"
    COMPANY_NAME "Fluidsynth LGPL"
    ORIGINAL_FILENAME "libfluidsynth.dll"
    FILE_DESCRIPTION "Fluidsynth"
)
endif ( WIN32 AND NOT MINGW )

link_directories (
    ${GLIB_LIBDIR}
    ${GLIB_LIBRARY_DIRS}
)


# note: by default this target creates a shared object (or dll). To build a
# static library instead, set the option BUILD_SHARED_LIBS to FALSE.
add_library ( fluidsynth STATIC
    ${config_SOURCES}
    ${fluid_windows_SOURCES}
    ${libfluidsynth_SOURCES}
    ${public_HEADERS}
    ${public_main_HEADER}
    ${VersionFilesOutputVariable}
)

if ( LIBFLUID_CPPFLAGS )
  set_target_properties ( libfluidsynth
    PROPERTIES COMPILE_FLAGS ${LIBFLUID_CPPFLAGS} )
endif ( LIBFLUID_CPPFLAGS )

target_link_libraries ( fluidsynth STATIC
    ${GLIB_LIBRARIES}
    ${GMODULE_LIBRARIES}
    ${WINDOWS_LIBS}
    ${LIBFLUID_LIBS}
)
