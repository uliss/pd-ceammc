add_executable(test_ext_ui test_ui_base.h)
target_sources(test_ext_ui PRIVATE test_ui.h test_ui.cpp)

target_compile_definitions(test_ext_ui
    PRIVATE -DPROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
    PRIVATE -DTEST_DIR="${CMAKE_CURRENT_BINARY_DIR}")

target_include_directories(test_ext_ui
    PRIVATE
        "${PROJECT_SOURCE_DIR}/ceammc/ext/src/ui"
        "${CMAKE_CURRENT_SOURCE_DIR}/.."
        "${CATCH_INCLUDE_DIR}")

target_link_libraries(test_ext_ui PUBLIC
    tests_main_lib
    ceammc_ui
    ceammc_env
    ceammc_core
    puredata-core)

add_test(NAME test_ext_ui
    COMMAND test_ext_ui
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})


macro(ceammc_ui_test name)
    string(REGEX REPLACE "[~]$" "" output ${name})
    string(REGEX REPLACE "[.]" "_" output ${output})

    target_sources(test_ext_ui PRIVATE "test_ui_${output}.cpp")
endmacro()


ceammc_ui_test("factory")
ceammc_ui_test("aview")
ceammc_ui_test("bang")
ceammc_ui_test("colorpanel")
ceammc_ui_test("display")
ceammc_ui_test("dsp~")
ceammc_ui_test("gain~")
ceammc_ui_test("icon")
ceammc_ui_test("incdec")
ceammc_ui_test("knob")
ceammc_ui_test("keyboard")
ceammc_ui_test("label")
ceammc_ui_test("link")
ceammc_ui_test("matrix")
ceammc_ui_test("menu")
ceammc_ui_test("meter")
ceammc_ui_test("number")
ceammc_ui_test("number_tilde")
ceammc_ui_test("radio")
ceammc_ui_test("rslider")
ceammc_ui_test("polar")
ceammc_ui_test("preset")
ceammc_ui_test("slider2d")
ceammc_ui_test("sliders")
ceammc_ui_test("tab")
ceammc_ui_test("toggle")
