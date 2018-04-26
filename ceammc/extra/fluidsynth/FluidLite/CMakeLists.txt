include(FindPkgConfig)
include(GNUInstallDirs)

list(APPEND HEADERS include/fluidlite.h)

list(APPEND SCOPED_HEADERS
    include/fluidsynth/types.h
    include/fluidsynth/settings.h
    include/fluidsynth/synth.h
    include/fluidsynth/sfont.h
    include/fluidsynth/ramsfont.h
    include/fluidsynth/log.h
    include/fluidsynth/misc.h
    include/fluidsynth/mod.h
    include/fluidsynth/gen.h
    include/fluidsynth/voice.h
    include/fluidsynth/version.h
)

list(APPEND SOURCES
    src/fluid_chan.c
    src/fluid_chorus.c
    src/fluid_conv.c
    src/fluid_defsfont.c
    src/fluid_dsp_float.c
    src/fluid_gen.c
    src/fluid_hash.c
    src/fluid_list.c
    src/fluid_mod.c
    src/fluid_ramsfont.c
    src/fluid_rev.c
    src/fluid_settings.c
    src/fluid_synth.c
    src/fluid_sys.c
    src/fluid_tuning.c
    src/fluid_voice.c
)

# Vorbis
set(LIBVORBIS_VERSION "1.3.6")
list(APPEND SOURCES
    libvorbis-${LIBVORBIS_VERSION}/lib/vorbisenc.c
    libvorbis-${LIBVORBIS_VERSION}/lib/info.c
    libvorbis-${LIBVORBIS_VERSION}/lib/analysis.c
    libvorbis-${LIBVORBIS_VERSION}/lib/bitrate.c
    libvorbis-${LIBVORBIS_VERSION}/lib/block.c
    libvorbis-${LIBVORBIS_VERSION}/lib/codebook.c
    libvorbis-${LIBVORBIS_VERSION}/lib/envelope.c
    libvorbis-${LIBVORBIS_VERSION}/lib/floor0.c
    libvorbis-${LIBVORBIS_VERSION}/lib/floor1.c
    libvorbis-${LIBVORBIS_VERSION}/lib/lookup.c
    libvorbis-${LIBVORBIS_VERSION}/lib/lpc.c
    libvorbis-${LIBVORBIS_VERSION}/lib/lsp.c
    libvorbis-${LIBVORBIS_VERSION}/lib/mapping0.c
    libvorbis-${LIBVORBIS_VERSION}/lib/mdct.c
    libvorbis-${LIBVORBIS_VERSION}/lib/psy.c
    libvorbis-${LIBVORBIS_VERSION}/lib/registry.c
    libvorbis-${LIBVORBIS_VERSION}/lib/res0.c
    libvorbis-${LIBVORBIS_VERSION}/lib/sharedbook.c
    libvorbis-${LIBVORBIS_VERSION}/lib/smallft.c
    libvorbis-${LIBVORBIS_VERSION}/lib/vorbisfile.c
    libvorbis-${LIBVORBIS_VERSION}/lib/window.c
    libvorbis-${LIBVORBIS_VERSION}/lib/synthesis.c
)
list(APPEND LIBVORBIS_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/libvorbis-${LIBVORBIS_VERSION}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/libvorbis-${LIBVORBIS_VERSION}/lib
)

# OGG
set(LIBOGG_VERSION "1.3.3")
list(APPEND SOURCES
    libogg-${LIBOGG_VERSION}/src/bitwise.c
    libogg-${LIBOGG_VERSION}/src/framing.c
    )
set(LIBOGG_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/libogg-${LIBOGG_VERSION}/include)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${LIBOGG_INCLUDE_DIRS})
include_directories(${LIBVORBIS_INCLUDE_DIRS})

add_library(fluidlite STATIC ${SOURCES})
set_property(TARGET fluidlite PROPERTY C_STANDARD 99)
