add_executable(test_ext_data test_data_base.h)

target_compile_definitions(test_ext_data
    PRIVATE -DPROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
    PRIVATE -DTEST_DIR="${CMAKE_CURRENT_BINARY_DIR}")

target_include_directories(test_ext_data
    PRIVATE
        "${PROJECT_SOURCE_DIR}/ceammc/ext/src/data"
        "${CMAKE_CURRENT_SOURCE_DIR}/.."
        "${CATCH_INCLUDE_DIR}")

target_link_libraries(test_ext_data PUBLIC
    tests_main_lib
    ceammc_data
    ceammc_core
    puredata-core)

add_test(NAME test_ext_data
    COMMAND test_ext_data
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

function(add_data_test src)
    target_sources(test_ext_data PRIVATE "test_data_${src}.cpp")
endfunction()


add_data_test(bimap)
add_data_test(color)
add_data_test(dict)
add_data_test(dict_contains)
add_data_test(dict_each)
add_data_test(dict_from_list)
add_data_test(dict_get)
add_data_test(dict_is)
add_data_test(dict_keys)
add_data_test(dict_pass)
add_data_test(dict_reject)
add_data_test(dict_size)
add_data_test(dict_to_list)
add_data_test(global_dict)
add_data_test(global_list)
add_data_test(global_mlist)
add_data_test(global_set)
add_data_test(fifo)
add_data_test(float)
add_data_test(int)
add_data_test(list)
add_data_test(local_dict)
add_data_test(local_float)
add_data_test(local_int)
add_data_test(local_list)
add_data_test(local_mlist)
add_data_test(mlist)
add_data_test(set)
add_data_test(set_difference)
add_data_test(set_intersection)
add_data_test(set_equal)
add_data_test(set_size)
add_data_test(set_symdiff)
add_data_test(set_union)
