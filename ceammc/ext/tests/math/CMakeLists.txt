add_executable(test_ext_math test_math_base.h)

target_compile_definitions(test_ext_math
    PRIVATE -DPROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
    PRIVATE -DTEST_DIR="${CMAKE_CURRENT_BINARY_DIR}")

target_include_directories(test_ext_math
    PRIVATE
        "${PROJECT_SOURCE_DIR}/ceammc/ext/src/math"
        "${CMAKE_CURRENT_SOURCE_DIR}/.."
        "${CATCH_INCLUDE_DIR}")

target_link_libraries(test_ext_math PUBLIC
    tests_main_lib
    ceammc_math
    ceammc_core
    puredata-core)

add_test(NAME test_ext_math
    COMMAND test_ext_math
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

function(add_math_test src)
    target_sources(test_ext_math PRIVATE "test_math_${src}.cpp")
endfunction()

add_math_test(and)
add_math_test(approx)
add_math_test(cdiv_tilde)
add_math_test(cmul_tilde)
add_math_test(div)
add_math_test(expr)
add_math_test(gcd)
add_math_test(lcm)
add_math_test(mul)
add_math_test(or)
add_math_test(polyeval)
add_math_test(sync_ops)
