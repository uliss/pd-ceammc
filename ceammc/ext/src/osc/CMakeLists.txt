set(OSC_SOURCES osc_blit.cpp)

macro(ceammc_osc_obj_external name)
    list(APPEND OSC_SOURCES "osc_${name}.cpp")
    ceammc_faust_gen_obj(osc ${name})
endmacro()

ceammc_osc_obj_external(impulse)
ceammc_osc_obj_external(pulse)
ceammc_osc_obj_external(saw)
ceammc_osc_obj_external(saw4)
ceammc_osc_obj_external(sin)
ceammc_osc_obj_external(sinfb)
ceammc_osc_obj_external(square)
ceammc_osc_obj_external(tri)

include_directories(${CMAKE_SOURCE_DIR}/ceammc/extra/stk/stk/include)

add_library(ceammc_osc STATIC mod_osc.cpp ${OSC_SOURCES})
target_link_libraries(ceammc_osc stk)

set_target_properties(ceammc_osc PROPERTIES COMPILE_DEFINITIONS "FAUST_MACRO")
