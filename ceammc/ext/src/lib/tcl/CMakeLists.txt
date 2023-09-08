add_custom_command(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/ceammc_texteditor.h
    COMMAND $<TARGET_FILE:tcl2cxx> ${CMAKE_CURRENT_SOURCE_DIR}/ceammc_texteditor.tcl > ceammc_texteditor.h
    DEPENDS ceammc_texteditor.tcl
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    VERBATIM)
