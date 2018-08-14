set(OSC_SOURCES osc_blit.cpp)
macro(ceammc_osc_extension name)
    list(APPEND OSC_SOURCES "osc_${name}.cpp")
    ceammc_faust_gen(osc ${name} simple_pd_control_ext)
endmacro()

ceammc_osc_extension(impulse)
ceammc_osc_extension(pulse)
ceammc_osc_extension(saw)
ceammc_osc_extension(sinfb)
ceammc_osc_extension(square)
ceammc_osc_extension(tri)

include_directories(${CMAKE_SOURCE_DIR}/ceammc/extra/stk/stk/include)

add_library(ceammc_osc STATIC mod_osc.cpp ${OSC_SOURCES})
target_link_libraries(ceammc_osc stk)

set_target_properties(ceammc_osc PROPERTIES COMPILE_DEFINITIONS "FAUST_MACRO")
if(UNIX)
    set_target_properties(ceammc_osc PROPERTIES COMPILE_FLAGS "-fPIC")
endif()
