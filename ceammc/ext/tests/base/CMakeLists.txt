add_executable(test_ext_base test_base_base.h)

target_compile_definitions(test_ext_base
    PRIVATE -DPROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
    PRIVATE -DTEST_DIR="${CMAKE_CURRENT_BINARY_DIR}")

target_include_directories(test_ext_base
    PRIVATE
        "${PROJECT_SOURCE_DIR}/ceammc/ext/src/base"
        "${CMAKE_CURRENT_SOURCE_DIR}/.."
        "${CATCH_INCLUDE_DIR}")

target_link_libraries(test_ext_base PUBLIC
    tests_main_lib
    ceammc_base
    ceammc_core
    puredata-core)

add_test(NAME test_ext_base
    COMMAND test_ext_base
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

function(add_base_test src)
    target_sources(test_ext_base PRIVATE "test_base_${src}.cpp")
endfunction()


add_base_test(canvas_current)
add_base_test(canvas_top)
add_base_test(dac)
add_base_test(expand_env)
add_base_test(function)
add_base_test(gain)
add_base_test(is_data)
add_base_test(is_file)
add_base_test(log)
add_base_test(matrix)
add_base_test(metro_pattern)
add_base_test(metro_seq)
add_base_test(mix)
add_base_test(msg)
add_base_test(msg_onload)
add_base_test(msg_sched)
add_base_test(obj_props)
add_base_test(patch_args)
add_base_test(patch_deps)
add_base_test(prop)
add_base_test(prop_declare)
add_base_test(prop_get)
add_base_test(prop_split)
add_base_test(prop_random)
add_base_test(radio)
add_base_test(replace)
add_base_test(snd_file)
add_base_test(xfade)
add_base_test(xfade2)
add_base_test(z_tilde)
