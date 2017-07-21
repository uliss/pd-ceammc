function(make_pddoc_lib)
    set(_OPTIONS_ARGS)
    set(_ONE_VALUE_ARGS NAME VERSION HOME)
    set(_MULTI_VALUE_ARGS FILES)

    cmake_parse_arguments(_LIB "${_OPTIONS_ARGS}" "${_ONE_VALUE_ARGS}" "${_MULTI_VALUE_ARGS}" ${ARGN})
    
    set(_LIB_PD_FILES)
    set(_LIB_XLET_DB_FILES)
    set(_LIB_PDDOC_FILES)
    
    foreach(f ${_LIB_FILES})
        set(fname "${CMAKE_CURRENT_BINARY_DIR}/${f}-help.pd")
        set(fname_pddoc "${CMAKE_CURRENT_SOURCE_DIR}/${f}.pddoc")
        list(APPEND _LIB_PD_FILES ${fname})
        list(APPEND _LIB_PDDOC_FILES ${fname_pddoc})
        add_custom_command(
            OUTPUT "${fname}"
            DEPENDS "${fname_pddoc}"
            COMMAND ${PD_DOC2PD}
                 --force
                 --version "${_LIB_VERSION}"
                 --website "${_LIB_HOME}"
                 --xlet-db "${CMAKE_SOURCE_DIR}/ceammc/ext/doc/ceammc.db"
                "${fname_pddoc}" ${fname}
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${fname} ${CMAKE_CURRENT_SOURCE_DIR}
            VERBATIM)
    endforeach()

    add_custom_command(
        OUTPUT "${_LIB_NAME}_lib.xml"
        DEPENDS ${_LIB_PDDOC_FILES}
        COMMAND ${PD_MAKELIB}
            --library "${_LIB_NAME}"
            --version "${_LIB_VERSION}"
            --output "${_LIB_NAME}_lib.xml" ${_LIB_PDDOC_FILES}
            --search-path ${CMAKE_CURRENT_SOURCE_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${_LIB_NAME}_lib.xml" ${CMAKE_CURRENT_SOURCE_DIR})

    add_custom_command(
        OUTPUT "${_LIB_NAME}-help.pd"
        DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/${_LIB_NAME}_lib.xml"
        COMMAND ${PD_LIB2PD} "${CMAKE_CURRENT_SOURCE_DIR}/${_LIB_NAME}_lib.xml"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${_LIB_NAME}-help.pd" ${CMAKE_CURRENT_SOURCE_DIR})

#    add_custom_target(ceammc_pddoc_cat
#        DEPENDS ${DOC_PD_FILES} "ceammc_lib.xml"
#        COMMAND ${PD_CAT2PD} "ceammc_lib.xml"
#        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

    add_custom_target(${_LIB_NAME}_pddoc
        DEPENDS ${_LIB_PD_FILES} "${_LIB_NAME}_lib.xml" "${_LIB_NAME}-help.pd"
        COMMAND cat "*-xlet_db.txt" | sort > "${_LIB_NAME}.db"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${_LIB_NAME}.db" ${CMAKE_CURRENT_SOURCE_DIR})
endfunction()
