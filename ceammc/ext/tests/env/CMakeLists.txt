add_executable(test_ext_env test_env_base.h)

target_compile_definitions(test_ext_env
    PRIVATE -DPROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
    PRIVATE -DTEST_DIR="${CMAKE_CURRENT_BINARY_DIR}")

target_include_directories(test_ext_env
    PRIVATE
        "${PROJECT_SOURCE_DIR}/ceammc/ext/src/env"
        "${CMAKE_CURRENT_SOURCE_DIR}/.."
        "${CATCH_INCLUDE_DIR}")

target_link_libraries(test_ext_env PUBLIC
    tests_main_lib
    ceammc_env
    ceammc_array
    ceammc_core
    puredata-core)

add_test(NAME test_ext_env
    COMMAND test_ext_env
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

function(add_env_test src)
    target_sources(test_ext_env PRIVATE "test_env_${src}.cpp")
endfunction()

add_env_test(adsr)
add_env_test(ar)
add_env_test(asr)
add_env_test(env)
add_env_test(to_array)
add_env_test(to_vline)
add_env_test(tscale)
add_env_test(tshift)
