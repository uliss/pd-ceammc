add_executable(test_ext_random)
target_sources(test_ext_random
    PRIVATE test_random_base.h)

target_compile_definitions(test_ext_random
    PRIVATE -DPROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
    PRIVATE -DTEST_DIR="${CMAKE_CURRENT_BINARY_DIR}")

target_include_directories(test_ext_random
    PRIVATE
        "${PROJECT_SOURCE_DIR}/ceammc/ext/src/random"
        "${CMAKE_CURRENT_SOURCE_DIR}/.."
        "${CATCH_INCLUDE_DIR}")

target_link_libraries(test_ext_random PUBLIC
    tests_main_lib
    ceammc_random
    ceammc_core
    puredata-core)

add_test(NAME test_ext_random
    COMMAND test_ext_random
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

function(add_random_test src)
    target_sources(test_ext_random PRIVATE "test_random_${src}.cpp")
endfunction()

add_random_test(atom)
add_random_test(discrete)
add_random_test(float)
add_random_test(int)
add_random_test(linear)
add_random_test(pw_lin)
add_random_test(pw_const)
