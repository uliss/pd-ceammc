set(PDDOC_DB_FILE "${CMAKE_CURRENT_SOURCE_DIR}/../ceammc.db")
set(PDDOC_LIB_FILE "${CMAKE_CURRENT_SOURCE_DIR}/../ceammc_lib.xml")

set(DOC_PD_FILES)
set(DOC_XLET_DB_FILES)
set(DOC_PDDOC_FILES)

foreach(f ${DOC_FILES})
    set(fname "${CMAKE_CURRENT_BINARY_DIR}/${f}-help.pd")
    set(fname_pddoc "${CMAKE_CURRENT_SOURCE_DIR}/../${f}.pddoc")
    list(APPEND DOC_PD_FILES ${fname})
    list(APPEND DOC_PDDOC_FILES ${fname_pddoc})
    add_custom_command(
        OUTPUT "${fname}"
        DEPENDS "${fname_pddoc}"
        COMMAND ${PD_DOC2PD}
             --force
             --version "${CEAMMC_LIB_VERSION}"
             --website "${CEAMMC_LIB_HOME}"
             --xlet-db ${PDDOC_DB_FILE}
             --locale RU
             --write-xlets-db 0
            "${fname_pddoc}" ${fname}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${fname} ${CMAKE_CURRENT_SOURCE_DIR}
        VERBATIM COMMAND find ${CMAKE_CURRENT_BINARY_DIR} -name "${f}.*.pd" -exec cp {} ${CMAKE_CURRENT_SOURCE_DIR} ";"
    )
endforeach()

add_custom_target(ceammc_pddoc_ru
    DEPENDS ${DOC_PD_FILES} ${PDDOC_LIB_FILE}
    # COMMAND cat "*-xlet_db.txt" | sort > ceammc.db
    # COMMAND ${CMAKE_COMMAND} -E copy_if_different ceammc.db ${CMAKE_CURRENT_SOURCE_DIR}
)

# install all help files
foreach(_doc_file ${DOC_FILES})
    install(FILES "${_doc_file}-help.pd"
        DESTINATION ${DOC_INSTALL_PATH}/ru
        PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ)
    file(GLOB _xfiles "${_doc_file}-*.pd")
    # find extra files, for example: module.name.test1.pd
    foreach(_xfile ${_xfiles})
        if(NOT ${_xfile} MATCHES "-help\.pd$")
            install(FILES ${_xfile}
                DESTINATION ${DOC_INSTALL_PATH}/ru
                PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ)
        endif()
    endforeach()
endforeach()

# Documentation index install
install(FILES "index-help.pd" DESTINATION "${DOC_INSTALL_PATH}/ru"
    PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ)
