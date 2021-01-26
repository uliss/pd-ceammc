add_executable(test_ext_flt test_flt_base.h)

target_compile_definitions(test_ext_flt
    PRIVATE -DPROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
    PRIVATE -DTEST_DIR="${CMAKE_CURRENT_BINARY_DIR}")

target_include_directories(test_ext_flt
    PRIVATE
        "${PROJECT_SOURCE_DIR}/ceammc/ext/src/flt"
        "${CMAKE_CURRENT_SOURCE_DIR}/.."
        "${CATCH_INCLUDE_DIR}")

target_link_libraries(test_ext_flt PUBLIC
    tests_main_lib
    ceammc_flt
    ceammc_core
    puredata-core)

add_test(NAME test_ext_flt
    COMMAND test_ext_flt
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

function(add_flt_test src)
    target_sources(test_ext_flt PRIVATE "test_flt_${src}.cpp")
endfunction()

add_flt_test(freqz)
add_flt_test(lpf2biquad)
add_flt_test(median)
