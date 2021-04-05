add_executable(test_ext_path
    test_path_base.h
    test_path_file_size.cpp
)

target_compile_definitions(test_ext_path
    PRIVATE -DPROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
    PRIVATE -DTEST_DIR="${CMAKE_CURRENT_BINARY_DIR}")

target_include_directories(test_ext_path
    PRIVATE
        "${PROJECT_SOURCE_DIR}/ceammc/ext/src/path"
        "${CMAKE_CURRENT_SOURCE_DIR}/.."
        "${CATCH_INCLUDE_DIR}")

target_link_libraries(test_ext_path PUBLIC
    tests_main_lib
    ceammc_path
    ceammc_core
    puredata-core)

add_test(NAME test_ext_path
    COMMAND test_ext_path
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

function(add_path_test src)
    target_sources(test_ext_path PRIVATE "test_path_${src}.cpp")
endfunction()

add_path_test(exists)
add_path_test(is_dir)
add_path_test(lsdir)
add_path_test(normalize)
add_path_test(split)
