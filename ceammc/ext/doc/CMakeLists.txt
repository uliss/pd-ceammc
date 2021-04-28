# get commmit information
set(GIT_BRANCH "master")
set(GIT_COMMIT "")
string(TIMESTAMP BUILD_DATETIME "%d %m %Y UTC %H:%M:%S " UTC)

if(EXISTS "${CMAKE_SOURCE_DIR}/.git")
    find_program(_GIT git)
    if(_GIT)
        execute_process(
            COMMAND ${_GIT} symbolic-ref --short HEAD
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            OUTPUT_VARIABLE GIT_BRANCH
            OUTPUT_STRIP_TRAILING_WHITESPACE)
        execute_process(
            COMMAND ${_GIT} describe --tags
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            OUTPUT_VARIABLE GIT_COMMIT
            OUTPUT_STRIP_TRAILING_WHITESPACE)
    endif()

    message(STATUS "Build from source git:
        branch: ${GIT_BRANCH}
        commit: ${GIT_COMMIT}
        date:   ${BUILD_DATETIME}")
else()
    message(STATUS "Build from source tarball
        date:   ${BUILD_DATETIME}")
endif()

# generating about info page
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/about.pd ${CMAKE_CURRENT_BINARY_DIR}/about.pd @ONLY)

# doc file list
include(docs.cmake)

if(NOT PD_LIB2PD)
    message(STATUS "pd_lib2pd not found.")
else()
    message(STATUS "pd_lib2pd found at: ${PD_LIB2PD}")
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
                 --xlet-db "${CMAKE_CURRENT_SOURCE_DIR}/ceammc.db"
                "${fname_pddoc}" ${fname}
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${fname} ${CMAKE_CURRENT_SOURCE_DIR}
            VERBATIM COMMAND find ${CMAKE_CURRENT_BINARY_DIR} -name "${f}.*.pd" -exec cp {} ${CMAKE_CURRENT_SOURCE_DIR} ";")
    endforeach()

    add_custom_command(
        OUTPUT "ceammc_lib.xml"
        DEPENDS ${DOC_PDDOC_FILES}
        COMMAND ${PD_MAKELIB}
            --library ceammc
            --version "${CEAMMC_LIB_VERSION}"
            --output ceammc_lib.xml ${DOC_PDDOC_FILES}
            --search-path ${CMAKE_CURRENT_SOURCE_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ceammc_lib.xml ${CMAKE_CURRENT_SOURCE_DIR})

    add_custom_command(
        OUTPUT "ceammc-help.pd"
        DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/ceammc_lib.xml"
        COMMAND ${PD_LIB2PD} "${CMAKE_CURRENT_SOURCE_DIR}/ceammc_lib.xml"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "ceammc-help.pd" ${CMAKE_CURRENT_SOURCE_DIR})

    add_custom_target(ceammc_pddoc_cat
        DEPENDS ${DOC_PD_FILES} "ceammc_lib.xml"
        COMMAND ${PD_CAT2PD} "ceammc_lib.xml"
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

    add_custom_target(ceammc_pddoc
        DEPENDS ${DOC_PD_FILES} "ceammc_lib.xml" ceammc_pddoc_cat "ceammc-help.pd"
        COMMAND cat "*-xlet_db.txt" | sort > ceammc.db
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ceammc.db ${CMAKE_CURRENT_SOURCE_DIR})

    if(NOT PD_LIB2DEKEN)
        message(STATUS "pd_lib2deken not found.")
    else()
        message(STATUS "pd_lib2deken found at: ${PD_LIB2DEKEN}")
        add_custom_target(ceammc_pddoc_gen_deken_objlist
            DEPENDS "ceammc_lib.xml"
            COMMAND ${PD_LIB2DEKEN} --aliases --force --output "ceammc_deken_objlist.txt" "ceammc_lib.xml"
            WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
        add_dependencies(ceammc_pddoc ceammc_pddoc_gen_deken_objlist)
    endif()
endif()

if(WIN32)
    set(DOC_INSTALL_PATH "extra/ceammc")
else()
    set(DOC_INSTALL_PATH "${PD_DOC_INSTALL_PATH}/extra/ceammc")
endif()

# install all help files
foreach(_doc_file ${DOC_FILES})
    install(FILES "${_doc_file}-help.pd"
        DESTINATION ${DOC_INSTALL_PATH}
        PERMISSIONS OWNER_READ GROUP_READ WORLD_READ)
    file(GLOB _xfiles "${_doc_file}.*.pd")
    # find extra files, for example: module.name.test1.pd
    foreach(_xfile ${_xfiles})
        if(NOT ${_xfile} MATCHES "-help\.pd$")
            install(FILES ${_xfile}
                DESTINATION ${DOC_INSTALL_PATH}
                PERMISSIONS OWNER_READ GROUP_READ WORLD_READ)
        endif()
    endforeach()
endforeach()

# About Pd page
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/about.pd" DESTINATION ${DOC_INSTALL_PATH}
    PERMISSIONS OWNER_READ GROUP_READ WORLD_READ)

# Documentation index install
install(FILES "index-help.pd" DESTINATION "${DOC_INSTALL_PATH}/.."
    PERMISSIONS OWNER_READ GROUP_READ WORLD_READ)

# extra doc file: presets, sound examples etc.
set(EXTRA_DOC_FILES prs.txt stargazing.mod)
file(GLOB _PDDOC_WAV "*.wav")
file(GLOB _PDDOC_EXAMPLES "snd.file.*.pd")
file(GLOB _PDDOC_GLITCH "*.glitch")

# install extra files
install(FILES ${EXTRA_DOC_FILES}
        ${_PDDOC_WAV}
        ${_PDDOC_EXAMPLES}
        ${_PDDOC_GLITCH}
        ui.plot~.lin1-help.pd
        ui.plot~.lin2-help.pd
        ui.plot~.lin3-help.pd
        ui.plot~.ln-help.pd
        ui.plot~.log2-help.pd
        ui.plot~.log10-help.pd
        poly.obj.pd
        poly.osc~.pd
    DESTINATION ${DOC_INSTALL_PATH}
    PERMISSIONS OWNER_READ GROUP_READ WORLD_READ)

install(
    CODE "file( GLOB _PD_CATEGORY_HELP_FILES \"${CMAKE_CURRENT_SOURCE_DIR}/ceammc*-help.pd\" )"
    CODE "file( INSTALL \${_PD_CATEGORY_HELP_FILES}
        DESTINATION \"${CMAKE_INSTALL_PREFIX}/${DOC_INSTALL_PATH}\")"
)

if(APPLE)
    find_program(TIDY tidy PATHS /usr/local/bin /usr/bin
        NO_DEFAULT_PATH
        NO_CMAKE_ENVIRONMENT_PATH
        NO_SYSTEM_ENVIRONMENT_PATH
        NO_CMAKE_SYSTEM_PATH
        NO_CMAKE_PATH)
else()
    find_program(TIDY tidy)
endif()

if(TIDY)
    message(STATUS "HTMLtidy found: ${TIDY}. The 'ceammc_pddoc_format' target available.")
    add_custom_target(ceammc_pddoc_format
        COMMAND ls *.pddoc | while read f\;
        do
            ${TIDY} -quiet -xml --indent auto
            --indent-spaces 4 --indent-cdata no --wrap 100 --wrap-sections no $$f > "${CMAKE_CURRENT_BINARY_DIR}/$$f" \;
            ${CMAKE_COMMAND} -E copy_if_different
                "${CMAKE_CURRENT_BINARY_DIR}/$$f" . \;
            echo "Format pddoc file: $$f" \;
        done
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
endif()


file(GLOB _PDDOC_FILES "*.pddoc")
file(GLOB _PDDOC_XML_FILES "*.xml")
add_custom_target(pddoc_files SOURCES ${_PDDOC_FILES} ${_PDDOC_XML_FILES} ceammc.db ls_missing_doc.py)
file(GLOB _PDDOC_PD_FILES "*.pd")
add_custom_target(pddoc_pd_files SOURCES ${_PDDOC_PD_FILES})
file(GLOB _PDDOC_FAUST_FILES "faust/*")
add_custom_target(pddoc_faust_files SOURCES ${_PDDOC_FAUST_FILES})

# install HOA library docs and examples
install(DIRECTORY hoa DESTINATION ${DOC_INSTALL_PATH})
install(DIRECTORY faust DESTINATION ${DOC_INSTALL_PATH})

add_subdirectory(sound)
add_subdirectory(sf2)

find_program(RSYNC_EXEC rsync)

if(APPLE)
    if(RSYNC_EXEC)
        # note: trailing slash after CMAKE_CURRENT_SOURCE_DIR is important!
        add_custom_target(dev_pddoc
            COMMAND ${RSYNC_EXEC} -av --include '*.pd' --exclude '*'
                ${CMAKE_CURRENT_SOURCE_DIR}/
                ${BUNDLE_FULL_PATH}/Contents/Resources/extra/ceammc
            DEPENDS ceammc_pddoc)
    endif()
endif()
