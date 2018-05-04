set(SYNTH_SOURCES)
set(SYNTH_STK_SOURCES
    stksynth.cpp
    synth_fgrain.cpp
    synth_shakers.cpp
    synth_wurley.cpp
    synth_bthree.cpp
    synth_rhodey.cpp)

macro(ceammc_synth_extension name)
    list(APPEND SYNTH_SOURCES "synth_${name}.cpp")
    ceammc_faust_gen(synth ${name})
endmacro()

ceammc_synth_extension(birds)
ceammc_synth_extension(church_bell)
#ceammc_synth_extension(dx7)
ceammc_synth_extension(dubdub)
ceammc_synth_extension(eguitar)
ceammc_synth_extension(ks)
ceammc_synth_extension(marimba)
ceammc_synth_extension(risset_arp)
ceammc_synth_extension(risset_tone)

include_directories(${CMAKE_SOURCE_DIR}/ceammc/extra/stk/stk/include)

add_library(ceammc_synth STATIC mod_synth.cpp ${SYNTH_SOURCES} ${SYNTH_STK_SOURCES})
target_link_libraries(ceammc_synth stk)
set_target_properties(ceammc_synth PROPERTIES COMPILE_DEFINITIONS "FAUST_MACRO")
if(UNIX)
    set_target_properties(ceammc_synth PROPERTIES COMPILE_FLAGS "-fPIC")
endif()
