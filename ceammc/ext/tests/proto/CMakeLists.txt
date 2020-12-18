add_executable(test_ext_proto test_proto_base.h)

target_include_directories(test_ext_proto
    PRIVATE
        "${PROJECT_SOURCE_DIR}/ceammc/ext/src/proto"
        "${CMAKE_CURRENT_SOURCE_DIR}/..")

target_link_libraries(test_ext_proto PUBLIC
    ceammc_proto
    tests_main_lib
    ceammc_core
    puredata-core)

add_test(NAME test_ext_proto
    COMMAND test_ext_proto
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

function(add_proto_test name)
    target_sources(test_ext_proto PRIVATE "test_proto_${name}.cpp")
endfunction()

add_proto_test(firmata)
add_proto_test(firmata_parser)
add_proto_test(firmata_pin)
add_proto_test(xtouch)
