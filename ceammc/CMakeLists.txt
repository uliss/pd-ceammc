# we need 1.50.0 for boost random uniform distribution
find_package(Boost 1.50.0 REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
add_subdirectory(extra/corrosion)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/extra/corrosion/cmake")

if(LibSndFile_FOUND)
    if(${LIBSNDFILE_VERSION} VERSION_LESS "1.30.0")
        set(CEAMMC_HAVE_LIBSNDFILE_OPUS TRUE)
    endif()
endif()

find_package(LibMagic)
if(LibMagic_FOUND)
    set(CEAMMC_HAVE_LIBMAGIC TRUE)
endif()

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/ceammc_config.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/ceammc_config.h
    @ONLY)

find_program(PD_DOC2PD pd_doc2pd)
find_program(PD_MAKELIB pd_makelibrary)
find_program(PD_LIB2PD pd_lib2pd)
find_program(PD_LIB2DEKEN pd_lib2deken)
find_program(PD_CAT2PD pd_cat2pd)

set(CEAMMC_INSTALL_PATH ${PD_INTERNAL_EXT_INSTALL_PATH}/ceammc)
set(CEAMMC_FAUST_TARGETS "" CACHE INTERNAL "")

add_subdirectory(extra)
add_subdirectory(gui)
add_subdirectory(ext)
add_subdirectory(distrib)
add_subdirectory(faust)
add_subdirectory(py)
add_subdirectory(scripts)
add_subdirectory(utils)

set(POT_TEMPLATE "${CMAKE_SOURCE_DIR}/po/template.pot")
find_program(XGETTEXT xgettext)

if(XGETTEXT)
    file(GLOB _CICM_FILES RELATIVE ${CMAKE_SOURCE_DIR}/po "${CMAKE_CURRENT_SOURCE_DIR}/ext/src/lib/cicm/Sources/*.cpp")
    file(GLOB _CEAMMC_FILES RELATIVE ${CMAKE_SOURCE_DIR}/po "${CMAKE_CURRENT_SOURCE_DIR}/ext/src/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/ext/src/*.h")
    file(GLOB _CEAMMC_LIB_FILES RELATIVE ${CMAKE_SOURCE_DIR}/po "${CMAKE_CURRENT_SOURCE_DIR}/ext/src/lib/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/ext/src/lib/*.h")
    file(GLOB _UI_FILES RELATIVE ${CMAKE_SOURCE_DIR}/po "${CMAKE_CURRENT_SOURCE_DIR}/ext/src/ui/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/ext/src/ui/*.h")
    file(GLOB _HOA_FILES RELATIVE ${CMAKE_SOURCE_DIR}/po "${CMAKE_CURRENT_SOURCE_DIR}/ext/src/spat/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/ext/src/spat/*.h")
    file(GLOB _TCL_PLUGIN_FILES RELATIVE ${CMAKE_SOURCE_DIR}/po "${CMAKE_CURRENT_SOURCE_DIR}/gui/plugins/ceammc_*-plugin.tcl")
    list(APPEND _TCL_PLUGIN_FILES "${CMAKE_CURRENT_SOURCE_DIR}/gui/plugins/completion-plugin/completion-plugin.tcl")

    set(XGETTEXT_EMAIL "serge.uliss at gmail.com")
    set(XGETTEXT_COPYRIGHT "This file is put in the public domain")
    add_custom_target(ceammc_messages
        COMMAND ${XGETTEXT} --join-existing --from-code=UTF-8
            --language=C --keyword=_
            --sort-by-file --output=${POT_TEMPLATE}
            --add-location=file
            --package-name="Pure Data" --package-version=${PD_TEXT_VERSION_FULL}
            --copyright-holder="${XGETTEXT_COPYRIGHT}"
            --foreign-user --msgid-bugs-address=${XGETTEXT_EMAIL}
            ${_CICM_FILES} ${_CEAMMC_FILES} ${_CEAMMC_LIB_FILES} ${_UI_FILES} ${_HOA_FILES}
        COMMAND ${XGETTEXT} --join-existing --from-code=UTF-8
            --language=Tcl --keyword=_
            --sort-by-file --output=${POT_TEMPLATE}
            --add-location=file
            --package-name="Pure Data" --package-version=${PD_TEXT_VERSION_FULL}
            --copyright-holder=${XGETTEXT_COPYRIGHT}
            --foreign-user --msgid-bugs-address=${XGETTEXT_EMAIL}
            ${_TCL_PLUGIN_FILES}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}

    )
endif()

add_custom_target(ceammc_extra_files
    SOURCES
        CHANGELOG.md
        cppcheck_suppressions.txt
        dev_info_Darwin.txt
        dev_info_Linux.txt
        dev_info_Windows.txt
)


set(DEV_INFO_TXT "${CMAKE_CURRENT_SOURCE_DIR}/dev_info_${CMAKE_SYSTEM_NAME}.txt")

if(${CMAKE_VERSION} VERSION_LESS "3.18.0")
    add_custom_target(dev COMMAND cat "${DEV_INFO_TXT}")
else()
    add_custom_target(dev COMMAND ${CMAKE_COMMAND} -E cat "${DEV_INFO_TXT}")
endif()

add_custom_target(dev_faust
    COMMAND ${CMAKE_COMMAND} -D ALL_TARGETS="${CEAMMC_FAUST_TARGETS}" -P "${CMAKE_CURRENT_SOURCE_DIR}/scripts/list_faust_targets.cmake")

add_custom_target(dev_graph
    COMMAND ${CMAKE_COMMAND} --graphviz=${PROJECT_BINARY_DIR}/ceammc_deps.dot ${PROJECT_SOURCE_DIR}
    COMMAND dot -Tpng ${PROJECT_BINARY_DIR}/ceammc_deps.dot > ${PROJECT_BINARY_DIR}/ceammc_deps.png
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
    USES_TERMINAL
)
