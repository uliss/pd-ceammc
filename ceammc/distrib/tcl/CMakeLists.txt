add_subdirectory(tkdnd)

install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/tkdnd/library/pkgIndex.tcl
        tkdnd/library/tkdnd.tcl
        tkdnd/library/tkdnd_generic.tcl
        tkdnd/library/tkdnd_compat.tcl
        tkdnd/library/tkdnd_macosx.tcl
        tkdnd/library/tkdnd_unix.tcl
        tkdnd/library/tkdnd_windows.tcl
        tkdnd/library/tkdnd_utils.tcl
    DESTINATION
        ${PD_GUI_PLUGINS_PATH}/tkdnd)

install(TARGETS tkdnd2.9.3
    RUNTIME DESTINATION ${PD_GUI_PLUGINS_PATH}/tkdnd
    LIBRARY DESTINATION ${PD_GUI_PLUGINS_PATH}/tkdnd
    ARCHIVE DESTINATION ${PD_GUI_PLUGINS_PATH}/tkdnd
)
