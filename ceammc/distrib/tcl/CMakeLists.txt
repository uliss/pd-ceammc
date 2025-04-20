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

if(UNIX AND NOT APPLE)
    set_target_properties(tkdnd2.9.3 PROPERTIES SOVERSION "2.9.3")
endif()

if(APPLE)
    # tkimg macports
    find_path(TKIMG_PATH
        NAMES pkgIndex.tcl
        PATH_SUFFIXES Img1.4.16
        PATHS /opt/local/lib
        NO_DEFAULT_PATH
    )

    if(TKIMG_PATH)
        message(STATUS "tkimg found: ${TKIMG_PATH}")
        install(DIRECTORY ${TKIMG_PATH}/
            DESTINATION ${PD_GUI_PLUGINS_PATH}/Img1.4.16
        )
    else()
        message(STATUS "tkimg not found")
    endif()
endif()
