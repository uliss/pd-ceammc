add_custom_target(buttonbar
    COMMAND sh ${CMAKE_CURRENT_SOURCE_DIR}/generate_buttonbar_plugin.sh ceammc_buttonbar-plugin.tcl
    SOURCES generate_buttonbar_plugin.sh
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    USES_TERMINAL)

file(GLOB GUI_PLUGINS "${CMAKE_CURRENT_SOURCE_DIR}/*-plugin.tcl")
list(APPEND GUI_PLUGINS "${CMAKE_CURRENT_SOURCE_DIR}/ceammc_popup_menu_tree.tcl")
file(GLOB GUI_COMPLETION_PLUGIN "${CMAKE_CURRENT_SOURCE_DIR}/completion-plugin/*-plugin.tcl")
list(APPEND GUI_COMPLETION_PLUGIN "${CMAKE_CURRENT_SOURCE_DIR}/completion-plugin/completion.cfg")
list(APPEND GUI_COMPLETION_PLUGIN "${CMAKE_CURRENT_SOURCE_DIR}/completion-plugin/manual.pd")
list(APPEND GUI_COMPLETION_PLUGIN "${CMAKE_CURRENT_SOURCE_DIR}/completion-plugin/COPYING")

install(FILES ${GUI_PLUGINS} ${GUI_COMPLETION_PLUGIN} DESTINATION ${PD_GUI_PLUGINS_PATH}/ceammc)

add_custom_target(gui_plugins SOURCES
    ${GUI_PLUGINS}
    ${GUI_COMPLETION_PLUGIN}
    buttonbar.tmpl.tcl
    check_popup_list.py
    ceammc_popup_menu_ceammc.tcl
    ceammc_popup_menu_extra.tcl
    ceammc_popup_menu_vanilla.tcl
)
add_custom_target(popup_gen
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/gen_ceammc_popup_list.py
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/gen_popup_menu.sh
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    SOURCES gen_ceammc_popup_list.py gen_popup_menu.sh)
