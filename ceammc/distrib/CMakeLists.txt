add_subdirectory(win)
add_subdirectory(mac)
add_subdirectory(linux)
add_subdirectory(docker)

set(MAKELIB_SCRIPT "")

if(WITH_DOUBLE_PRECISION)
    set(PD_FLOAT_SIZE 64)
else()
    set(PD_FLOAT_SIZE 32)
endif()

if(APPLE)
    set(DYLIBBUNDLER "${CMAKE_CURRENT_BINARY_DIR}/mac/dylibbundler")

    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/update_cpp_src_from_doc.sh
                   ${PROJECT_BINARY_DIR}/update_cpp_src_from_doc.sh @ONLY)
endif()

if(UNIX AND NOT APPLE)
    set(MAKELIB_SCRIPT "${PROJECT_BINARY_DIR}/dist/make_lib_linux.sh")
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/linux/make_lib_linux.sh ${MAKELIB_SCRIPT} @ONLY)
endif()

find_program(DEKEN deken PATH $ENV{HOME}/bin)

if(DEKEN)
    message(STATUS "deken program found: ${DEKEN}. Target `deken_package` is available")

    if(WIN32)
        set(DEKEN_MAKE_PACKAGE   bash -c "${PROJECT_BINARY_DIR}/dist/deken_make_package.sh")
        set(DEKEN_UPLOAD_PACKAGE bash -c "${PROJECT_BINARY_DIR}/dist/deken_upload_package.sh")
    else()
        set(DEKEN_MAKE_PACKAGE   "${PROJECT_BINARY_DIR}/dist/deken_make_package.sh")
        set(DEKEN_UPLOAD_PACKAGE "${PROJECT_BINARY_DIR}/dist/deken_upload_package.sh")
    endif()

    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/deken_make_package.sh
        "${PROJECT_BINARY_DIR}/dist/deken_make_package.sh" @ONLY)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/deken_upload_package.sh
        "${PROJECT_BINARY_DIR}/dist/deken_upload_package.sh" @ONLY)

    add_custom_target(deken_package
        COMMAND ${DEKEN_MAKE_PACKAGE}
        WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/dist"
        USES_TERMINAL)

    add_custom_target(deken_upload
        COMMAND ${DEKEN_UPLOAD_PACKAGE}
        WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/dist"
        USES_TERMINAL)
endif()

# deken scripts
add_custom_target(ceammc_distrib_deken_sources
    SOURCES deken_make_package.sh deken_upload_package.sh)


# documentation check
set(DOC_CHECK_SCRIPTS
    check_all_pddoc_refs.sh
    check_doc_external.py
    doc_check_methods.sh
    fill_doc_properties.py
    gen_doc_template.py
    list_all_doc_externals.sh
    update_pddoc.py
)

add_custom_target(distrib_doc_check_scripts SOURCES ${DOC_CHECK_SCRIPTS})

foreach(_s ${DOC_CHECK_SCRIPTS})
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/${_s} ${PROJECT_BINARY_DIR} @ONLY)
endforeach()

add_custom_target(ceammc_doc_check_methods
    COMMAND ${PROJECT_BINARY_DIR}/doc_check_methods.sh
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR})

# ceammc release patch files
set(RELEASE_TEMPLATE "template_release_${CEAMMC_LIB_VERSION}.pd")
set(RELEASE_PATCH "release_${CEAMMC_LIB_VERSION}.pd")
configure_file(${RELEASE_TEMPLATE} ${CMAKE_CURRENT_SOURCE_DIR}/${RELEASE_PATCH} @ONLY)

if(WIN32)
    set(DOC_INSTALL_PATH "extra/ceammc")
else()
    set(DOC_INSTALL_PATH "${PD_DOC_INSTALL_PATH}/extra/ceammc")
endif()

file(GLOB _RELEASE_FILES "release_*.pd")
foreach(f ${_RELEASE_FILES})
    install(FILES ${f}
        DESTINATION ${DOC_INSTALL_PATH}
        PERMISSIONS OWNER_READ GROUP_READ WORLD_READ)
endforeach()

# bashrc function for testing
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/bashrc_tests bashrc_tests @ONLY)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/dev_sync_tcl.sh
               ${PROJECT_BINARY_DIR}/dist/dev_sync_tcl.sh @ONLY)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/dev_sync_pd.sh
              ${PROJECT_BINARY_DIR}/dist/dev_sync_pd.sh @ONLY)

add_custom_target(dev_tcl
    ${PROJECT_BINARY_DIR}/dist/dev_sync_tcl.sh)

add_custom_target(dev_pd
    ${PROJECT_BINARY_DIR}/dist/dev_sync_pd.sh $<TARGET_FILE:pd>
    USES_TERMINAL)
