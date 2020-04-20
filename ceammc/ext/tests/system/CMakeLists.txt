set(TEST_OUTPUT_DIR "..")

add_executable(test_ext_system)
target_sources(test_ext_system
    PRIVATE test_system_base.h)

target_compile_definitions(test_ext_system
    PRIVATE -DPROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
    PRIVATE -DTEST_DIR="${CMAKE_CURRENT_BINARY_DIR}")

target_include_directories(test_ext_system
    PRIVATE
        "${PROJECT_SOURCE_DIR}/ceammc/ext/src/system"
        "${PROJECT_SOURCE_DIR}/ceammc/extra"
        "${CMAKE_CURRENT_SOURCE_DIR}/.."
        "${CATCH_INCLUDE_DIR}")

target_link_libraries(test_ext_system PUBLIC
    tests_main_lib
    ceammc_system
    ceammc_core
    ceammc_sound
    puredata-core
)

add_test(NAME test_ext_system
    COMMAND test_ext_system
    WORKING_DIRECTORY ${TEST_OUTPUT_DIR})

set_target_properties(test_ext_system PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY  "${TEST_OUTPUT_DIR}"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${TEST_OUTPUT_DIR}"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${TEST_OUTPUT_DIR}"
)

function(add_system_test src)
    target_sources(test_ext_system PRIVATE "test_system_${src}.cpp")
endfunction()

add_executable(test_exec test_exec.cpp)
add_dependencies(test_ext_system test_exec)

add_system_test(exec)
add_system_test(getenv)
