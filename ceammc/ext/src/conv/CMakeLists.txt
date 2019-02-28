include_directories(${CMAKE_SOURCE_DIR}/src)

set(CONV_SOURCES "baserangeconverter.cpp")

macro(ceammc_conv_external name)
    list(APPEND CONV_SOURCES "conv_${name}.cpp")
endmacro()

# MIDI
ceammc_conv_external(cc2amp)

ceammc_conv_external(lin2curve)
ceammc_conv_external(lin2exp)
ceammc_conv_external(lin2lin)

ceammc_conv_external(samp2sec)
ceammc_conv_external(sec2str)
ceammc_conv_external(sec2samp)
ceammc_conv_external(str2sec)

ceammc_conv_external(bpm2ms)
ceammc_conv_external(bpm2sec)
ceammc_conv_external(bpm2hz)

ceammc_conv_external(pitch2midi)
ceammc_conv_external(amp2dbfs)
ceammc_conv_external(dbfs2amp)

ceammc_conv_external(midi2freq)
ceammc_conv_external(list2props)

add_library(ceammc_conv STATIC mod_conv.h mod_conv.cpp ${CONV_SOURCES})
if(UNIX)
    set_target_properties(ceammc_conv PROPERTIES COMPILE_FLAGS "-fPIC")
endif()
