include_directories(${CMAKE_SOURCE_DIR}/src)

set(FLOW_SOURCES "")

macro(ceammc_flow_extension name)
    list(APPEND FLOW_SOURCES "flow_${name}.cpp")
endmacro()

ceammc_flow_extension(change)
ceammc_flow_extension(group)
ceammc_flow_extension(pass)
ceammc_flow_extension(pass_if)
ceammc_flow_extension(reject)
ceammc_flow_extension(reject_if)
ceammc_flow_extension(split)
ceammc_flow_extension(sync)

add_library(ceammc_flow STATIC mod_flow.h mod_flow.cpp ${FLOW_SOURCES})
if(UNIX)
    set_target_properties(ceammc_flow PROPERTIES COMPILE_FLAGS "-fPIC")
endif()
