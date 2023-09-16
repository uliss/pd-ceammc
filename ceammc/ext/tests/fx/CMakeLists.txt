add_executable(test_ext_fx test_fx_base.h)

if(WITH_DOUBLE_PRECISION)
    add_definitions(-DFAUSTFLOAT=double)
endif()

target_include_directories(test_ext_fx
    PRIVATE
        "${PROJECT_SOURCE_DIR}/ceammc/ext/src/fx"
        "${CMAKE_CURRENT_SOURCE_DIR}/..")

target_link_libraries(test_ext_fx PUBLIC
    tests_main_lib
    ceammc_fx
    ceammc_core
    puredata-core)

add_test(NAME test_ext_fx
    COMMAND test_ext_fx
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

function(add_fx_test src)
    target_sources(test_ext_fx PRIVATE "test_fx_${src}.cpp")
endfunction()

add_fx_test(looper)
add_fx_test(stutter)
add_fx_test(tapiir)
