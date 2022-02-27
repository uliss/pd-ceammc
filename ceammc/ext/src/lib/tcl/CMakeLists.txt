add_custom_target(ceammc_tcl_gen
    COMMAND $<TARGET_FILE:tcl2cxx> ${CMAKE_CURRENT_SOURCE_DIR}/ceammc_texteditor.tcl > ceammc_texteditor.h
    SOURCES ceammc_texteditor.tcl
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
