add_library(ceammc_nui STATIC
    common.cpp
    factory.cpp
    layout.cpp
    model.cpp
    nui.cpp
    property.cpp
    tk_view_impl.cpp
    view.cpp
    widget.cpp
)

target_include_directories(ceammc_nui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_include_directories(ceammc_nui PUBLIC ${PROJECT_SOURCE_DIR}/src)

add_custom_target(nui_tcl_gen
    COMMAND ${PROJECT_SOURCE_DIR}/ceammc/scripts/filecontent_to_cxx_header.sh tcl_nui.tcl
    SOURCES tcl_nui.tcl
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

set(MODELS
    box_model.yaml
    frame_model.yaml
    label_model.yaml
    slider_model.yaml
    toggle_model.yaml
    vu_model.yaml
)
set(GEN_MODEL ${CMAKE_CURRENT_SOURCE_DIR}/gen_model_data.py)

add_custom_target(nui_model_gen
    COMMAND ${GEN_MODEL} box_model.yaml > box_model.h
    COMMAND ${GEN_MODEL} frame_model.yaml > frame_model.h
    COMMAND ${GEN_MODEL} label_model.yaml > label_model.h
    COMMAND ${GEN_MODEL} slider_model.yaml > slider_model.h
    COMMAND ${GEN_MODEL} toggle_model.yaml > toggle_model.h
    COMMAND ${GEN_MODEL} vu_model.yaml > vu_model.h
    SOURCES ${MODELS}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

