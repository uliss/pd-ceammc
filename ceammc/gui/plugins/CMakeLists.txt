add_custom_target(buttonbar
    COMMAND sh ${CMAKE_CURRENT_SOURCE_DIR}/generate_buttonbar_plugin.sh ceammc_buttonbar-plugin.tcl
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB GUI_PLUGINS "${CMAKE_CURRENT_SOURCE_DIR}/*-plugin.tcl")
list(APPEND GUI_PLUGINS "${CMAKE_CURRENT_SOURCE_DIR}/ceammc_popup_menu_tree.tcl")
list(APPEND GUI_PLUGINS "${CMAKE_CURRENT_SOURCE_DIR}/completion.cfg")

install(FILES ${GUI_PLUGINS} DESTINATION ${PD_GUI_PLUGINS_PATH}/ceammc)

add_custom_target(gui_plugins SOURCES ${GUI_PLUGINS} check_popup_list.py)
