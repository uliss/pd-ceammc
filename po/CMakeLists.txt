file(GLOB _PO_FILES "*.po")

foreach(_po_file ${_PO_FILES})
    get_filename_component(_locale ${_po_file} NAME_WE)
    execute_process(COMMAND ${GETTEXT_MSGFMT_EXECUTABLE} --tcl -l ${_locale} -d ${CMAKE_CURRENT_BINARY_DIR} ${_po_file})
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${_locale}.msg DESTINATION lib/puredata/po)
endforeach()
