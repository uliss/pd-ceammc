set(SYNTH_SOURCES)
set(SYNTH_STK_SOURCES
    faust_synth_harpsichord.cpp
    faust_synth_piano.cpp
    faust_synth_stk.cpp
    stksynth.cpp
    synth_bthree.cpp
    synth_fgrain.cpp
    synth_rhodey.cpp
    synth_shakers.cpp
    synth_sitar.cpp
    synth_wurley.cpp
)

macro(ceammc_synth_obj name)
    list(APPEND SYNTH_SOURCES "synth_${name}.cpp")
    ceammc_faust_gen_obj(synth ${name} JSON)
endmacro()

ceammc_synth_obj(birds)
ceammc_synth_obj(church_bell)
ceammc_synth_obj(dubdub)
ceammc_synth_obj(eguitar)
ceammc_synth_obj(english_bell)
ceammc_synth_obj(french_bell)
ceammc_synth_obj(german_bell)
ceammc_synth_obj(glass_harm)
ceammc_synth_obj(glitch)
ceammc_synth_obj(harpsichord)
ceammc_synth_obj(ks)
ceammc_synth_obj(marimba)
ceammc_synth_obj(piano)
ceammc_synth_obj(risset_arp)
ceammc_synth_obj(risset_tone)
ceammc_synth_obj(russian_bell)
ceammc_synth_obj(standard_bell)
ceammc_synth_obj(snare)

list(APPEND SYNTH_SOURCES "synth_dx7.cpp")
ceammc_faust_gen_obj(synth dx7 JSON OCPP)

add_library(ceammc_synth STATIC mod_synth.cpp ${SYNTH_SOURCES} ${SYNTH_STK_SOURCES})

target_link_libraries(ceammc_synth PRIVATE stk glitch-core)
target_include_directories(ceammc_synth PRIVATE
    ${PROJECT_SOURCE_DIR}/ceammc/extra/stk/stk/include
)

set_target_properties(ceammc_synth PROPERTIES COMPILE_DEFINITIONS "FAUST_MACRO")
