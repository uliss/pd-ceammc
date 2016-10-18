add_custom_target(buttonbar
    COMMAND sh ${CMAKE_CURRENT_SOURCE_DIR}/generate_buttonbar_plugin.sh ceammc_buttonbar-plugin.tcl
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
