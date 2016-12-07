find_program(PD_XML2PD pd_xml2pd)

set(DOC_FILES is_any is_bang)

if(NOT PD_XML2PD)
    message(STATUS "pd_xml2pd not found.")
else()
    message(STATUS "pd_xml2pd found at: ${PD_XML2PD}")
    set(DOC_PD_FILES)
    set(DOC_XLET_DB_FILES)
    foreach(f ${DOC_FILES})
        set(fname "${CMAKE_CURRENT_SOURCE_DIR}/${f}-help.pd")
        set(fname_xlet "${CMAKE_CURRENT_SOURCE_DIR}/${f}-xlet_db.txt")
        list(APPEND DOC_PD_FILES ${fname})
        list(APPEND DOC_XLET_DB_FILES ${fname_xlet})
        add_custom_command(
            OUTPUT ${fname}
            DEPENDS "${f}.pddoc"
            COMMAND ${PD_XML2PD}
                 --force
                 --version "${CEAMMC_LIB_VERSION}"
                 --website "${CEAMMC_LIB_HOME}"
                "${f}.pddoc" ${fname}
            WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
    endforeach()

    add_custom_command(
        OUTPUT "ceammc.db"
        DEPENDS ${DOC_XLET_DB_FILES}
        COMMAND cat "*-xlet_db.txt" | sort > ceammc.db
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

    add_custom_target(ceammc_pddoc
        DEPENDS ${DOC_PD_FILES} "ceammc.db"
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
endif()

