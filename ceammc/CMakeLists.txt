set(CEAMMC_LIB_VERSION_MAJOR 0)
set(CEAMMC_LIB_VERSION_MINOR 9)
set(CEAMMC_LIB_VERSION_PATCH 1)
set(CEAMMC_LIB_VERSION "${CEAMMC_LIB_VERSION_MAJOR}.${CEAMMC_LIB_VERSION_MINOR}.${CEAMMC_LIB_VERSION_PATCH}")
set(CEAMMC_LIB_VERSION ${CEAMMC_LIB_VERSION} PARENT_SCOPE)
set(CEAMMC_LIB_HOME "https://github.com/uliss/pure-data")

# we need 1.50.0 for boost random uniform distribution
find_package(Boost 1.50.0 REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/ceammc_config.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/ceammc_config.h
    @ONLY)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

find_program(PD_DOC2PD pd_doc2pd)
find_program(PD_MAKELIB pd_makelibrary)
find_program(PD_LIB2PD pd_lib2pd)
find_program(PD_LIB2DEKEN pd_lib2deken)
find_program(PD_CAT2PD pd_cat2pd)

set(CEAMMC_INSTALL_PATH ${PD_INTERNAL_EXT_INSTALL_PATH}/ceammc)

add_subdirectory(extra)
add_subdirectory(gui)
add_subdirectory(ext)
add_subdirectory(distrib)
add_subdirectory(faust)
add_subdirectory(py)
add_subdirectory(scripts)

set(POT_TEMPLATE "${CMAKE_SOURCE_DIR}/po/template.pot")
find_program(XGETTEXT xgettext)

if(XGETTEXT)
    file(GLOB _CICM_FILES RELATIVE ${CMAKE_SOURCE_DIR}/po "${CMAKE_CURRENT_SOURCE_DIR}/ext/src/lib/cicm/Sources/*.cpp")
    file(GLOB _CEAMMC_FILES RELATIVE ${CMAKE_SOURCE_DIR}/po "${CMAKE_CURRENT_SOURCE_DIR}/ext/src/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/ext/src/*.h")
    file(GLOB _CEAMMC_LIB_FILES RELATIVE ${CMAKE_SOURCE_DIR}/po "${CMAKE_CURRENT_SOURCE_DIR}/ext/src/lib/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/ext/src/lib/*.h")
    file(GLOB _UI_FILES RELATIVE ${CMAKE_SOURCE_DIR}/po "${CMAKE_CURRENT_SOURCE_DIR}/ext/src/ui/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/ext/src/ui/*.h")
    file(GLOB _HOA_FILES RELATIVE ${CMAKE_SOURCE_DIR}/po "${CMAKE_CURRENT_SOURCE_DIR}/ext/src/spat/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/ext/src/spat/*.h")

    add_custom_target(ceammc_messages
        COMMAND ${XGETTEXT} --join-existing --from-code=UTF-8
            --language=C --keyword=_
            --sort-by-file --output=${POT_TEMPLATE}
            --add-location=file
            --package-name="Pure Data" --package-version=${PD_TEXT_VERSION_FULL}
            --copyright-holder='This file is put in the public domain'
            --foreign-user --msgid-bugs-address=pd-dev@iem.at
            ${_CICM_FILES} ${_CEAMMC_FILES} ${_CEAMMC_LIB_FILES} ${_UI_FILES} ${_HOA_FILES}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endif()

add_custom_target(ceammc_extra_files SOURCES CHANGELOG.md cppcheck_suppressions.txt)


