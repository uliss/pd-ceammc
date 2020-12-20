add_executable(test_ext_wrapper test_wrapper_base.h)

target_compile_definitions(test_ext_wrapper
    PRIVATE -DPROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
    PRIVATE -DTEST_MIDI_DIR="${CMAKE_CURRENT_BINARY_DIR}"
    PRIVATE -DTEST_SRC_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

target_include_directories(test_ext_wrapper
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/.."
        "${PROJECT_SOURCE_DIR}/ceammc/ext/class-wrapper/src"
        "${CATCH_INCLUDE_DIR}")

target_link_libraries(test_ext_wrapper PUBLIC
    tests_main_lib
    wrapper_lib
    ceammc_core
    puredata-core)

add_test(NAME test_ext_wrapper
    COMMAND test_ext_wrapper
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

function(add_wrapper_test src)
    target_sources(test_ext_wrapper PRIVATE "test_wrapper_${src}.cpp")
endfunction()


add_wrapper_test(class)
add_wrapper_test(fn_traits)
add_wrapper_test(method)
add_wrapper_test(static_method)
add_wrapper_test(tuple)
