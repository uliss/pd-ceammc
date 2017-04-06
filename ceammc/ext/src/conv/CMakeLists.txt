set(CONV_SOURCES "")

macro(ceammc_conv_extension name)
    list(APPEND CONV_SOURCES "conv_${name}.cpp")
    ceammc_cxx_extension(conv ${name})
endmacro()

# BPF CONVERTERS
ceammc_conv_extension(bpf2env)
ceammc_conv_extension(bpf2list)
ceammc_conv_extension(bpf2vline)
ceammc_conv_extension(env2bpf)
ceammc_conv_extension(list2bpf)
ceammc_conv_extension(vline2bpf)
ceammc_conv_extension(bpf2array)
ceammc_conv_extension(lin2lin)

add_library(ceammc_conv STATIC mod_conv.h mod_conv.cpp ${CONV_SOURCES})
if(UNIX)
    set_target_properties(ceammc_conv PROPERTIES COMPILE_FLAGS "-fPIC")
endif()
