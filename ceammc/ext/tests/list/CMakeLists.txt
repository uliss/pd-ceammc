set(CATCH_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../Catch2/catch)
add_library(Catch2::Catch IMPORTED INTERFACE)
#set_property(Catch2::Catch PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${CATCH_INCLUDE_DIR}")

add_library(lib_catch2_main STATIC ../tests_main.cpp)

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
    Catch2::Catch
    lib_catch2_main
    tests_main_lib
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
add_test(any_of)
add_test(append)
