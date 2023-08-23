add_executable(test_ext_system test_system_base.h)

target_compile_definitions(test_ext_system
    PRIVATE -DPROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
    PRIVATE -DTEST_DIR="${CMAKE_CURRENT_BINARY_DIR}"
    PRIVATE -DTEST_EXEC="$<TARGET_FILE:test_exec>")

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
    puredata-core
)

add_test(NAME test_ext_system
    COMMAND test_ext_system
    WORKING_DIRECTORY ${TEST_OUTPUT_DIR})

function(add_system_test src)
    target_sources(test_ext_system PRIVATE "test_system_${src}.cpp")
endfunction()

add_executable(test_exec test_exec.cpp)
add_dependencies(test_ext_system test_exec)

add_system_test(colorpanel)
add_system_test(exec)
add_system_test(getenv)
