add_executable(test_ext_flow)
target_sources(test_ext_flow
    PRIVATE test_flow_base.h)

target_compile_definitions(test_ext_flow
    PRIVATE -DPROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
    PRIVATE -DTEST_DIR="${CMAKE_CURRENT_BINARY_DIR}")

target_include_directories(test_ext_flow
    PRIVATE
        "${PROJECT_SOURCE_DIR}/ceammc/ext/src/flow"
        "${CMAKE_CURRENT_SOURCE_DIR}/.."
        "${CATCH_INCLUDE_DIR}")

target_link_libraries(test_ext_flow PUBLIC
    tests_main_lib
    ceammc_base
    ceammc_flow
    ceammc_core
    puredata-core)

add_test(NAME test_ext_flow
    COMMAND test_ext_flow
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

function(add_flow_test src)
    target_sources(test_ext_flow PRIVATE "test_flow_${src}.cpp")
endfunction()

add_flow_test(append)
add_flow_test(change)
add_flow_test(count)
add_flow_test(delay)
add_flow_test(dup)
add_flow_test(gate)
add_flow_test(group)
add_flow_test(less)
add_flow_test(less_eq)
add_flow_test(demultiplex)
add_flow_test(demux_tilde)
add_flow_test(demux2_tilde)
add_flow_test(match)
add_flow_test(multiplex)
add_flow_test(mux_tilde)
add_flow_test(mux2_tilde)
add_flow_test(once)
add_flow_test(pack)
add_flow_test(pass)
add_flow_test(pass_if)
add_flow_test(pipe)
add_flow_test(reject)
add_flow_test(route)
add_flow_test(select)
add_flow_test(split)
add_flow_test(speedlim)
add_flow_test(stack)
add_flow_test(sync_pack)

target_sources(test_ext_flow PRIVATE
    test_seq_arp.cpp
    test_seq_bangs.cpp
    test_seq_counter.cpp
    test_seq_matrix.cpp
    test_seq_nbangs.cpp
    test_seq_phasor.cpp
    test_seq_sequencer.cpp
    test_seq_toggles.cpp
)
