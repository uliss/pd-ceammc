set(FLOW_SOURCES mod_flow.h mod_flow.cpp
    conway_life.cpp
    route_float.cpp
    seq_arp.cpp
    seq_bangs.cpp
    seq_base.cpp
    seq_counter.cpp
    seq_life.cpp
    seq_matrix.cpp
    seq_nbangs.cpp
    seq_phasor.cpp
    seq_sequencer.cpp
    seq_toggles.cpp
)

macro(ceammc_flow_external name)
    list(APPEND FLOW_SOURCES "flow_${name}.cpp")
endmacro()

ceammc_flow_external(append)
ceammc_flow_external(change)
ceammc_flow_external(compare_base)
ceammc_flow_external(count)
ceammc_flow_external(delay)
ceammc_flow_external(demultiplex)
ceammc_flow_external(demultiplex2_tilde)
ceammc_flow_external(demultiplex_tilde)
ceammc_flow_external(dup)
ceammc_flow_external(gate)
ceammc_flow_external(greater)
ceammc_flow_external(greater_eq)
ceammc_flow_external(group)
ceammc_flow_external(interval)
ceammc_flow_external(less)
ceammc_flow_external(less_eq)
ceammc_flow_external(list2many)
ceammc_flow_external(ring)
ceammc_flow_external(loop)
ceammc_flow_external(match)
ceammc_flow_external(mem)
ceammc_flow_external(multiplex)
ceammc_flow_external(multiplex2_tilde)
ceammc_flow_external(multiplex_tilde)
ceammc_flow_external(once)
ceammc_flow_external(pack)
ceammc_flow_external(pass)
ceammc_flow_external(pass_if)
ceammc_flow_external(pipe)
ceammc_flow_external(queue)
ceammc_flow_external(reject)
ceammc_flow_external(reject_if)
ceammc_flow_external(route)
ceammc_flow_external(select)
ceammc_flow_external(space)
ceammc_flow_external(speedlim)
ceammc_flow_external(split)
ceammc_flow_external(stack)
ceammc_flow_external(sync)
ceammc_flow_external(sync_pack)
ceammc_flow_external(tee_tilde)

add_library(ceammc_flow STATIC ${FLOW_SOURCES})

target_include_directories(ceammc_flow PUBLIC ${PROJECT_SOURCE_DIR}/src)
target_include_directories(ceammc_flow PRIVATE
    $<TARGET_PROPERTY:re2,INCLUDE_DIRECTORIES>
    $<TARGET_PROPERTY:fmt,INCLUDE_DIRECTORIES>
    $<TARGET_PROPERTY:reflex,INCLUDE_DIRECTORIES>)

target_link_libraries(ceammc_flow PRIVATE ceammc_base) # for function calls
target_link_libraries(ceammc_flow PRIVATE re2)
