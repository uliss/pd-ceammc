add_executable(test_nui test_nui.h)

target_include_directories(test_nui
    PRIVATE
        "${PROJECT_SOURCE_DIR}/ceammc/ext/src/lib"
        "${CMAKE_CURRENT_SOURCE_DIR}/..")

target_link_libraries(test_nui PUBLIC
    tests_main_lib
    ceammc_nui
    ceammc_core
    puredata-core)

add_test(NAME test_nui
    COMMAND test_nui
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

function(add_nui_test src)
    target_sources(test_nui PRIVATE "test_nui_${src}.cpp")
endfunction()

add_nui_test(model)
add_nui_test(view)
