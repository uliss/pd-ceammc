include(PdExtension)
# for m_pd.h and others
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/ceammc/ext/src/lib)

pd_add_extension(NAME soundtouch~
    FILES soundtouch_tilde.cpp
    INTERNAL False
    LIBRARY ceammc
    LINK ceammc_core soundtouch) 

file(GLOB _PDDOC_FILES "*.pddoc")
file(GLOB _PDDOC_XML_FILES "*.xml")

set(DOC_FILES soundtouch~)

if(PD_LIB2PD)
    set(DOC_PD_FILES)
    set(DOC_XLET_DB_FILES)
    set(DOC_PDDOC_FILES)
    foreach(f ${DOC_FILES})
        set(fname "${CMAKE_CURRENT_BINARY_DIR}/${f}-help.pd")
        set(fname_pddoc "${CMAKE_CURRENT_SOURCE_DIR}/${f}.pddoc")
        list(APPEND DOC_PD_FILES ${fname})
        list(APPEND DOC_PDDOC_FILES ${fname_pddoc})
        add_custom_command(
            OUTPUT "${fname}"
            DEPENDS "${fname_pddoc}"
            COMMAND ${PD_DOC2PD}
                 --force
                 --version "${CEAMMC_LIB_VERSION}"
                 --website "${CEAMMC_LIB_HOME}"
                 --xlet-db "${CMAKE_SOURCE_DIR}/ceammc/ext/doc/ceammc.db"
                "${fname_pddoc}" ${fname}
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${fname} ${CMAKE_CURRENT_SOURCE_DIR}
            VERBATIM)
    endforeach()

    add_custom_command(
        OUTPUT "soundtouch_lib.xml"
        DEPENDS ${DOC_PDDOC_FILES}
        COMMAND ${PD_MAKELIB}
            --library soundtouch
            --version "0.1"
            --output soundtouch_lib.xml ${DOC_PDDOC_FILES}
            --search-path ${CMAKE_CURRENT_SOURCE_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different soundtouch_lib.xml ${CMAKE_CURRENT_SOURCE_DIR})

    add_custom_command(
        OUTPUT "soundtouch-help.pd"
        DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/soundtouch_lib.xml"
        COMMAND ${PD_LIB2PD} "${CMAKE_CURRENT_SOURCE_DIR}/soundtouch_lib.xml"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "soundtouch-help.pd" ${CMAKE_CURRENT_SOURCE_DIR})

#    add_custom_target(ceammc_pddoc_cat
#        DEPENDS ${DOC_PD_FILES} "ceammc_lib.xml"
#        COMMAND ${PD_CAT2PD} "ceammc_lib.xml"
#        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

    add_custom_target(soundtouch_pddoc
        DEPENDS ${DOC_PD_FILES} "soundtouch_lib.xml" "soundtouch-help.pd"
        COMMAND cat "*-xlet_db.txt" | sort > soundtouch.db
        COMMAND ${CMAKE_COMMAND} -E copy_if_different soundtouch.db ${CMAKE_CURRENT_SOURCE_DIR})
endif()

add_custom_target(sountouch_pddoc_files SOURCES ${_PDDOC_FILES} ${_PDDOC_XML_FILES})

