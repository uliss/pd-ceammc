add_executable(test_ext_spat)
target_sources(test_ext_spat
    PRIVATE test_spat_base.h)

target_compile_definitions(test_ext_spat
    PRIVATE -DPROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
    PRIVATE -DTEST_DIR="${CMAKE_CURRENT_BINARY_DIR}")

target_include_directories(test_ext_spat
    PRIVATE
        "${PROJECT_SOURCE_DIR}/ceammc/ext/src/spat"
        "${CMAKE_CURRENT_SOURCE_DIR}/.."
        "${PROJECT_SOURCE_DIR}/ceammc/extra/HoaLibrary-Light/Sources"
        "${EIGEN3_DIR}"
        "${CATCH_INCLUDE_DIR}")

target_link_libraries(test_ext_spat PUBLIC
    tests_main_lib
    ceammc_spat
    ceammc_core
    puredata-core)

add_test(NAME test_ext_spat
    COMMAND test_ext_spat
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

function(add_spat_test src)
    target_sources(test_ext_spat PRIVATE "test_spat_${src}.cpp")
endfunction()

add_spat_test(hoa_in)
add_spat_test(hoa_process)
add_spat_test(hoa_recomposer)
add_spat_test(pan_cos)
add_spat_test(pan_lin)
add_spat_test(pan_spread)
add_spat_test(pan_sqrt)
