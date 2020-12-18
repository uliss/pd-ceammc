add_executable(test_ext_preset test_preset_base.h)

target_compile_definitions(test_ext_preset
    PRIVATE -DPROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
    PRIVATE -DTEST_DIR="${CMAKE_CURRENT_BINARY_DIR}")

target_include_directories(test_ext_preset
    PRIVATE
        "${PROJECT_SOURCE_DIR}/ceammc/ext/src/preset"
        "${CMAKE_CURRENT_SOURCE_DIR}/.."
        "${CATCH_INCLUDE_DIR}")

target_link_libraries(test_ext_preset PUBLIC
    tests_main_lib
    ceammc_preset
    ceammc_core
    puredata-core)

add_test(NAME test_ext_preset
    COMMAND test_ext_preset
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

function(add_preset_test src)
    target_sources(test_ext_preset PRIVATE "test_preset_${src}.cpp")
endfunction()


add_preset_test(float)
add_preset_test(symbol)
add_preset_test(list)

add_preset_test(lib)
