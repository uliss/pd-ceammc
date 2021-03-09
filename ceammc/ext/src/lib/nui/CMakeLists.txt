add_custom_command(
    COMMAND $<TARGET_FILE:tcl2cxx> ${CMAKE_CURRENT_SOURCE_DIR}/nui_button.tcl > nui_button_tcl.h
    DEPENDS nui_button.tcl
    OUTPUT  nui_button_tcl.h)

add_custom_command(
    COMMAND $<TARGET_FILE:tcl2cxx> ${CMAKE_CURRENT_SOURCE_DIR}/nui_label.tcl > nui_label_tcl.h
    DEPENDS nui_label.tcl
    OUTPUT  nui_label_tcl.h)

add_custom_command(
    COMMAND $<TARGET_FILE:tcl2cxx> ${CMAKE_CURRENT_SOURCE_DIR}/nui_slider.tcl > nui_slider_tcl.h
    DEPENDS nui_slider.tcl
    OUTPUT  nui_slider_tcl.h)

add_custom_command(
    COMMAND $<TARGET_FILE:tcl2cxx> ${CMAKE_CURRENT_SOURCE_DIR}/tcl_nui.tcl > tcl_nui_tcl.h
    DEPENDS tcl_nui.tcl
    OUTPUT  tcl_nui_tcl.h)


add_library(ceammc_nui STATIC
    button_view.cpp
    common.cpp
    factory.cpp
    label_view.cpp
    layout.cpp
    model.cpp
    nui.cpp
    nui_button_tcl.h
    nui_label_tcl.h
    nui_slider_tcl.h
    property.cpp
    slider_view.cpp
    tcl_nui_tcl.h
    tk_view_impl.cpp
    view.cpp
    widget.cpp
)

target_include_directories(ceammc_nui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_include_directories(ceammc_nui PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(ceammc_nui PUBLIC ${PROJECT_SOURCE_DIR}/src)

set(GEN_MODEL ${CMAKE_CURRENT_SOURCE_DIR}/gen_model_data.py)

add_custom_target(nui_model_gen
    COMMAND ${GEN_MODEL} bar_model.yaml > bar_model.h
    COMMAND ${GEN_MODEL} box_model.yaml > box_model.h
    COMMAND ${GEN_MODEL} button_model.yaml > button_model.h
    COMMAND ${GEN_MODEL} frame_model.yaml > frame_model.h
    COMMAND ${GEN_MODEL} label_model.yaml > label_model.h
    COMMAND ${GEN_MODEL} slider_model.yaml > slider_model.h
    COMMAND ${GEN_MODEL} toggle_model.yaml > toggle_model.h
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(ceammc_nui PRIVATE ceammc_core puredata-core)

file(GLOB _YAML_FILES "*.yaml")
file(GLOB _TCL_FILES "*.tcl")
add_custom_target(nui_model_sources SOURCES ${_YAML_FILES} ${_TCL_FILES})
