macro(ceammc_dyn_external name)
    list(APPEND DYN_SOURCES "dyn_${name}.cpp")
    ceammc_faust_gen(dyn ${name})
endmacro()

macro(ceammc_dyn_obj_ext name)
    list(APPEND DYN_SOURCES "dyn_${name}.cpp")
    ceammc_faust_gen_obj(dyn ${name} JSON)
endmacro()

ceammc_dyn_external(limit)
ceammc_dyn_external(limit2)
ceammc_dyn_external(softclip)

ceammc_dyn_obj_ext(comp)
ceammc_dyn_obj_ext(comp2)
ceammc_dyn_obj_ext(gate)
ceammc_dyn_obj_ext(gate2)

add_library(ceammc_dyn STATIC mod_dyn.cpp ${DYN_SOURCES})
set_target_properties(ceammc_dyn PROPERTIES COMPILE_DEFINITIONS "FAUST_MACRO")
if(UNIX)
    set_target_properties(ceammc_dyn PROPERTIES COMPILE_FLAGS "-fPIC")
endif()
