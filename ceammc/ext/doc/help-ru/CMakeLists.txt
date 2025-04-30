set(PDDOC_DB_FILE "${CMAKE_CURRENT_SOURCE_DIR}/../ceammc.db")
set(PDDOC_LIB_FILE "${CMAKE_CURRENT_SOURCE_DIR}/../ceammc_lib.xml")
set(PDDOC_HELP_INDEX "${CMAKE_CURRENT_SOURCE_DIR}/ceammc-help.pd")

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

add_custom_target(ceammc_pddoc_cat_ru
    DEPENDS ${DOC_PD_FILES} ${PDDOC_LIB_FILE}
    COMMAND ${PD_CAT2PD}
        --locale RU
        ${PDDOC_LIB_FILE}
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

add_custom_command(
    OUTPUT  ${PDDOC_HELP_INDEX}
    DEPENDS ${PDDOC_LIB_FILE}
    COMMAND ${PD_LIB2PD}
        --xlet-db "${CMAKE_CURRENT_SOURCE_DIR}/.."
        --locale RU
        ${PDDOC_LIB_FILE}
    COMMAND ${CMAKE_COMMAND}
        -E copy_if_different ${PDDOC_HELP_INDEX}
        ${CMAKE_CURRENT_SOURCE_DIR}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_custom_target(ceammc_pddoc_ru
    DEPENDS ceammc_pddoc_cat_ru ${DOC_PD_FILES} ${PDDOC_LIB_FILE} ${PDDOC_HELP_INDEX}
)
