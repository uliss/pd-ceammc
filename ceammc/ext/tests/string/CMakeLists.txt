add_executable(test_ext_string)
target_sources(test_ext_string
    PRIVATE test_string_base.h)

target_compile_definitions(test_ext_string
    PRIVATE -DPROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
    PRIVATE -DTEST_DIR="${CMAKE_CURRENT_BINARY_DIR}")

target_include_directories(test_ext_string
    PRIVATE
        "${PROJECT_SOURCE_DIR}/ceammc/ext/src/string"
        "${CMAKE_CURRENT_SOURCE_DIR}/.."
        "${CATCH_INCLUDE_DIR}")

target_link_libraries(test_ext_string PUBLIC
    tests_main_lib
    ceammc_string
    ceammc_core
    puredata-core)

add_test(NAME test_ext_string
    COMMAND test_ext_string
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

function(add_string_test src)
    target_sources(test_ext_string PRIVATE "test_string_${src}.cpp")
endfunction()

target_sources(test_ext_string PRIVATE
    test_symbol_equal.cpp
    test_symbol_num_compare.cpp
    test_symbol_to_int.cpp)

# test list
add_string_test(contains)
add_string_test(ends_with)
add_string_test(equal)
add_string_test(format)
add_string_test(join)
add_string_test(length)
add_string_test(match)
add_string_test(remove)
add_string_test(replace)
add_string_test(split)
add_string_test(starts_with)
add_string_test(str)
add_string_test(substr)
add_string_test(to_symbol)
