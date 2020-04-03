set(CATCH_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../Catch2)
add_library(Catch2::Catch IMPORTED INTERFACE)
set_target_properties(Catch2::Catch PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${CATCH_INCLUDE_DIR}")

add_library(tests_main_lib2 STATIC
    ../tests_main.cpp
    ../test_approx.cpp
    ../test_datatypes.cpp
    ../test_external.cpp
    ../test_external_log_output.cpp
    ../test_wrappers.cpp
)

target_link_libraries(tests_main_lib2 PUBLIC Catch2::Catch)

add_executable(test_ext_list)
target_sources(test_ext_list
    PRIVATE test_list_base.h test_list_base.cpp)

target_compile_definitions(test_ext_list
    PRIVATE -DPROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
    PRIVATE -DTEST_DIR="${CMAKE_CURRENT_BINARY_DIR}")

target_include_directories(test_ext_list
    PRIVATE
        "${PROJECT_SOURCE_DIR}/ceammc/ext/src/list"
        "${CMAKE_CURRENT_SOURCE_DIR}/.."
        "${CATCH_INCLUDE_DIR}")

target_link_libraries(test_ext_list PUBLIC
    tests_main_lib2
    ceammc_list
    ceammc_core
    ceammc_sound
    puredata-core)

add_test(NAME test_ext_list
    COMMAND test_ext_list
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

function(add_test src)
    target_sources(test_ext_list PRIVATE "test_list_${src}.cpp")
endfunction()

# test list
add_test(all_of)
add_test(any_of)
add_test(append)
