file(GLOB _PO_FILES "*.po")
add_custom_target(po_files SOURCES ${_PO_FILES})

add_custom_target(messages_compile)

foreach(_po_file ${_PO_FILES})
    get_filename_component(_locale ${_po_file} NAME_WE)
    set(_msg_file ${CMAKE_CURRENT_BINARY_DIR}/${_locale}.msg)
    add_custom_target("${_locale}.msg"
                       COMMAND ${GETTEXT_MSGFMT_EXECUTABLE} --tcl -l ${_locale} -d ${CMAKE_CURRENT_BINARY_DIR} ${_po_file}
                       DEPENDS ${_po_file})
    add_dependencies(messages_compile "${_locale}.msg")

    execute_process(COMMAND ${GETTEXT_MSGFMT_EXECUTABLE} --tcl -l ${_locale} -d ${CMAKE_CURRENT_BINARY_DIR} ${_po_file})
    install(FILES ${_msg_file} DESTINATION ${PD_PO_INSTALL_PATH})
endforeach()

set(POT_TEMPLATE "${CMAKE_CURRENT_SOURCE_DIR}/template.pot")
find_program(XGETTEXT xgettext)

if(XGETTEXT)
    file(GLOB _TCL_FILES RELATIVE ${CMAKE_SOURCE_DIR}/po "${CMAKE_SOURCE_DIR}/tcl/*.tcl")
    file(GLOB _TCL_FILES2 RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.tcl")
    file(GLOB _TCL_FILES3 RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "${CMAKE_SOURCE_DIR}/ceammc/gui/plugins/*.tcl")

    add_custom_target(messages
        COMMAND ${XGETTEXT} --join-existing --from-code=UTF-8
            --language=Tcl --keyword=_
            --sort-by-file --output=${POT_TEMPLATE}
            --add-location=file
            --package-name="Pure Data" --package-version=${PD_TEXT_VERSION_FULL}
            --copyright-holder='This file is put in the public domain'
            --foreign-user --msgid-bugs-address=pd-dev@iem.at
            ${_TCL_FILES} ${_TCL_FILES2} ${_TCL_FILES3}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endif()

if(GETTEXT_MSGMERGE_EXECUTABLE)
    add_custom_target(messages_merge
        COMMAND
            ${GETTEXT_MSGMERGE_EXECUTABLE} --update --backup=none  --sort-output
                "${CMAKE_CURRENT_SOURCE_DIR}/ru.po" ${POT_TEMPLATE})
endif()
