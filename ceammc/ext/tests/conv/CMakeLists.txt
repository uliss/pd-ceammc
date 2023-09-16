add_executable(test_ext_conv test_conv_base.h)

target_compile_definitions(test_ext_conv
    PRIVATE -DPROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
    PRIVATE -DTEST_DIR="${CMAKE_CURRENT_BINARY_DIR}")

target_include_directories(test_ext_conv
    PRIVATE
        "${PROJECT_SOURCE_DIR}/ceammc/ext/src/conv"
        "${CMAKE_CURRENT_SOURCE_DIR}/.."
        "${CATCH_INCLUDE_DIR}")

target_link_libraries(test_ext_conv PUBLIC
    tests_main_lib
    ceammc_conv
    ceammc_core
    puredata-core)

add_test(NAME test_ext_conv
    COMMAND test_ext_conv
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

function(add_conv_test src)
    target_sources(test_ext_conv PRIVATE "test_conv_${src}.cpp")
endfunction()

add_conv_test(amp2dbfs)
add_conv_test(bar2ms)
add_conv_test(bits2bang)
add_conv_test(bits2int)
add_conv_test(bits2note)
add_conv_test(bits2pos)
add_conv_test(bpm2ms)
add_conv_test(color2rgba)
add_conv_test(char2morse)
add_conv_test(cc2amp)
add_conv_test(dbfs2amp)
add_conv_test(int2bits)
add_conv_test(lin2exp)
add_conv_test(lin2lin)
add_conv_test(lin2lin_tilde)
add_conv_test(list2props)
add_conv_test(note2guido)
add_conv_test(pitch2midi)
