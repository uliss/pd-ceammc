add_custom_command(
    COMMAND $<TARGET_FILE:tcl2cxx> ${CMAKE_CURRENT_SOURCE_DIR}/tcl_nui.tcl > tcl_nui_tcl.h
    DEPENDS tcl_nui.tcl
    OUTPUT  tcl_nui_tcl.h)


add_library(ceammc_nui STATIC
    common.cpp
    factory.cpp
    layout.cpp
    model.cpp
    nui.cpp
    property.cpp
    tcl_nui_tcl.h
    tk_view_impl.cpp
    view.cpp
    widget.cpp
    widget_factory.h
)

function(nui_add_view NAME)
    add_custom_command(
        COMMAND $<TARGET_FILE:tcl2cxx> ${CMAKE_CURRENT_SOURCE_DIR}/nui_${NAME}.tcl > nui_${NAME}_tcl.h
        DEPENDS nui_${NAME}.tcl
        OUTPUT  nui_${NAME}_tcl.h)
    target_sources(ceammc_nui PRIVATE ${NAME}_view.h ${NAME}_view.cpp nui_${NAME}_tcl.h)
endfunction()

nui_add_view(button)
nui_add_view(combobox)
nui_add_view(help_box)
nui_add_view(label)
nui_add_view(nentry)
nui_add_view(slider)

target_include_directories(ceammc_nui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_include_directories(ceammc_nui PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(ceammc_nui PUBLIC ${PROJECT_SOURCE_DIR}/src)
target_include_directories(ceammc_nui
    PRIVATE
        $<TARGET_PROPERTY:fmt,INCLUDE_DIRECTORIES>
)

set(GEN_MODEL ${CMAKE_CURRENT_SOURCE_DIR}/gen_model_data.py)

add_custom_target(nui_model_gen
    COMMAND ${GEN_MODEL} bar_model.yaml         > bar_model.h
    COMMAND ${GEN_MODEL} box_model.yaml         > box_model.h
    COMMAND ${GEN_MODEL} button_model.yaml      > button_model.h
    COMMAND ${GEN_MODEL} combobox_model.yaml    > combobox_model.h
    COMMAND ${GEN_MODEL} frame_model.yaml       > frame_model.h
    COMMAND ${GEN_MODEL} label_model.yaml       > label_model.h
    COMMAND ${GEN_MODEL} nentry_model.yaml      > nentry_model.h
    COMMAND ${GEN_MODEL} help_box_model.yaml    > help_box_model.h
    COMMAND ${GEN_MODEL} slider_model.yaml      > slider_model.h
    COMMAND ${GEN_MODEL} toggle_model.yaml      > toggle_model.h
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(ceammc_nui PRIVATE ceammc_core puredata-core)

file(GLOB _YAML_FILES "*.yaml")
file(GLOB _TCL_FILES "*.tcl")
add_custom_target(nui_model_sources SOURCES ${_YAML_FILES} ${_TCL_FILES})
