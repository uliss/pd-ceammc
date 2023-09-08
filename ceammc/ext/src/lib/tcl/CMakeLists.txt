add_custom_command(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/ceammc_texteditor.h
    COMMAND $<TARGET_FILE:tcl2cxx> ${CMAKE_CURRENT_SOURCE_DIR}/ceammc_texteditor.tcl > ceammc_texteditor.h
    DEPENDS ceammc_texteditor.tcl
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/ceammc_tooltips.h
    COMMAND $<TARGET_FILE:tcl2cxx> ${CMAKE_CURRENT_SOURCE_DIR}/ceammc_tooltips.tcl > ceammc_tooltips.h
    DEPENDS ceammc_tooltips.tcl
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_custom_target(ceammc_lib_tcl SOURCES ceammc_texteditor.tcl ceammc_tooltips.tcl)
