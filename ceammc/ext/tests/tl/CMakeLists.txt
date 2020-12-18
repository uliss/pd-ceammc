add_executable(test_ext_tl test_tl_base.cpp)

target_compile_definitions(test_ext_tl
    PRIVATE -DPROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
    PRIVATE -DTEST_DIR="${CMAKE_CURRENT_BINARY_DIR}"
    PRIVATE -DTEST_EXEC="$<TARGET_FILE:test_exec>")

target_include_directories(test_ext_tl
    PRIVATE
        "${PROJECT_SOURCE_DIR}/ceammc/ext/src/tl"
        "${CMAKE_CURRENT_SOURCE_DIR}/.."
        "${CATCH_INCLUDE_DIR}")

target_link_libraries(test_ext_tl PUBLIC
    tests_main_lib
    ceammc_tl
    ceammc_ui
    ceammc_core
    puredata-core
)

add_test(NAME test_ext_tl
    COMMAND test_ext_tl
    WORKING_DIRECTORY ${TEST_OUTPUT_DIR})

function(add_tl_test src)
    target_sources(test_ext_tl PRIVATE "test_tl_${src}.cpp")
endfunction()

add_tl_test(timeline_lib)
add_tl_test(transport)
add_tl_test(timeline)
add_tl_test(parser)
