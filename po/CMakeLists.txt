file(GLOB _PO_FILES "*.po")

foreach(_po_file ${_PO_FILES})
    get_filename_component(_locale ${_po_file} NAME_WE)
    execute_process(COMMAND ${GETTEXT_MSGFMT_EXECUTABLE} --tcl -l ${_locale} -d ${CMAKE_CURRENT_BINARY_DIR} ${_po_file})
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${_locale}.msg DESTINATION lib/puredata/po)
endforeach()

find_program(XGETTEXT xgettext)

if(XGETTEXT)
    file(GLOB _TCL_FILES RELATIVE ${CMAKE_SOURCE_DIR}/po "${CMAKE_SOURCE_DIR}/tcl/*.tcl")
    set(TEMPLATE "${CMAKE_CURRENT_SOURCE_DIR}/template.pot")
    add_custom_target(messages
        COMMAND ${XGETTEXT} --join-existing --from-code=UTF-8
            --directory="${CMAKE_SOURCE_DIR}/tcl"
            --language=Tcl --keyword=_
            --sort-by-file --output=${TEMPLATE}
            --package-name="Pure Data" --package-version=${PD_TEXT_VERSION_FULL}
            --copyright-holder='This file is put in the public domain'
            --foreign-user --msgid-bugs-address=pd-dev@iem.at ${_TCL_FILES})
endif()
