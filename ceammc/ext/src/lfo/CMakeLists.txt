set(LFO_SOURCES "")
macro(ceammc_lfo_extension name)
    list(APPEND LFO_SOURCES "lfo_${name}.cpp")
    ceammc_faust_gen(lfo ${name})
endmacro()

ceammc_lfo_extension(impulse)
ceammc_lfo_extension(tri)
ceammc_lfo_extension(square)


add_library(ceammc_lfo STATIC mod_lfo.cpp ${LFO_SOURCES})
set_target_properties(ceammc_lfo PROPERTIES COMPILE_DEFINITIONS "FAUST_MACRO")
if(UNIX)
    set_target_properties(ceammc_lfo PROPERTIES COMPILE_FLAGS "-fPIC")
endif()
