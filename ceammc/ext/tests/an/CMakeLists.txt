add_executable(test_ext_an test_an.h)

target_include_directories(test_ext_an
    PRIVATE
        "${PROJECT_SOURCE_DIR}/ceammc/ext/src/an"
        "${PROJECT_SOURCE_DIR}/ceammc/extra/aubio/aubio/src"
        "${CMAKE_CURRENT_SOURCE_DIR}/..")

target_link_libraries(test_ext_an PUBLIC
    tests_main_lib
    ceammc_analyze
    ceammc_core
    aubio
    puredata-core)

add_test(NAME test_ext_an
    COMMAND test_ext_an
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

function(add_an_test src)
    target_sources(test_ext_an PRIVATE "test_an_${src}.cpp")
endfunction()

add_an_test(onset)
