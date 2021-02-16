add_library(ceammc_nui STATIC
    nui.cpp
    model.cpp
    tk_view_impl.cpp
    view.cpp
)

target_include_directories(ceammc_nui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_include_directories(ceammc_nui PRIVATE ${PROJECT_SOURCE_DIR}/src)

add_custom_target(nui_tcl_gen
    COMMAND ${PROJECT_SOURCE_DIR}/ceammc/scripts/filecontent_to_cxx_header.sh tcl_nui.tcl
    SOURCES tcl_nui.tcl
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
