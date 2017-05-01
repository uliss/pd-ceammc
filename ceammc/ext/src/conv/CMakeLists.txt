include_directories(${CMAKE_SOURCE_DIR}/src)

set(CONV_SOURCES "baserangeconverter.cpp")

macro(ceammc_conv_extension name)
    list(APPEND CONV_SOURCES "conv_${name}.cpp")
endmacro()

# MIDI
ceammc_conv_extension(cc2amp)

# BPF CONVERTERS
ceammc_conv_extension(bpf2env)
ceammc_conv_extension(bpf2list)
ceammc_conv_extension(bpf2vline)
ceammc_conv_extension(env2bpf)
ceammc_conv_extension(list2bpf)
ceammc_conv_extension(vline2bpf)
ceammc_conv_extension(bpf2array)

ceammc_conv_extension(lin2curve)
ceammc_conv_extension(lin2exp)
ceammc_conv_extension(lin2lin)

ceammc_conv_extension(samp2sec)
ceammc_conv_extension(sec2str)
ceammc_conv_extension(sec2samp)
ceammc_conv_extension(str2sec)

ceammc_conv_extension(bpm2ms)
ceammc_conv_extension(bpm2sec)
ceammc_conv_extension(bpm2hz)


add_library(ceammc_conv STATIC mod_conv.h mod_conv.cpp ${CONV_SOURCES})
if(UNIX)
    set_target_properties(ceammc_conv PROPERTIES COMPILE_FLAGS "-fPIC")
endif()
