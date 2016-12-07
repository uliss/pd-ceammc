find_program(PD_XML2PD pd_xml2pd)

set(DOC_FILES is_any is_bang)

if(NOT PD_XML2PD)
    message(STATUS "pd_xml2pd not found.")
else()
    message(STATUS "pd_xml2pd found at: ${PD_XML2PD}")
    set(DOC_PD_FILES)
    foreach(f ${DOC_FILES})
        set(fname "${CMAKE_CURRENT_SOURCE_DIR}/${f}-help.pd")
        list(APPEND DOC_PD_FILES ${fname})
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

    message(STATUS ${DOC_PD_FILES})

    add_custom_target(ceammc_pddoc
        DEPENDS "${DOC_PD_FILES}"
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
endif()

