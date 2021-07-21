add_executable(test_ext_array test_array_base.h)

target_compile_definitions(test_ext_array
    PRIVATE -DPROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
    PRIVATE -DTEST_DIR="${CMAKE_CURRENT_BINARY_DIR}")

target_include_directories(test_ext_array
    PRIVATE
        "${PROJECT_SOURCE_DIR}/ceammc/ext/src/array"
        "${CMAKE_CURRENT_SOURCE_DIR}/.."
        "${CATCH_INCLUDE_DIR}")

target_link_libraries(test_ext_array PUBLIC
    tests_main_lib
    ceammc_array
    ceammc_core
    puredata-core)

if(WIN32)
    target_link_libraries(test_ext_array PRIVATE soxr)
endif()

add_test(NAME test_ext_array
    COMMAND test_ext_array
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

function(add_array_test src)
    target_sources(test_ext_array PRIVATE "test_array_${src}.cpp")
endfunction()

add_array_test(bpm)
add_array_test(circular)
add_array_test(copy)
add_array_test(do)
add_array_test(each)
add_array_test(fill)
add_array_test(grainer)
add_array_test(grain_expr)
add_array_test(hist)
add_array_test(mean)
add_array_test(minmax)
add_array_test(mod)
add_array_test(p2s)
add_array_test(play_tilde)
add_array_test(plot)
add_array_test(plot_geomspace)
add_array_test(plot_linspace)
add_array_test(plot_logspace)
add_array_test(plot_tilde)
add_array_test(resample)
add_array_test(rms)
add_array_test(s2p)
add_array_test(stddev)
add_array_test(stretch)
add_array_test(sum)
add_array_test(sum2)
add_array_test(variance)
add_array_test(vplay)
add_array_test(window)
