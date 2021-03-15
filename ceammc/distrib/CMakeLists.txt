add_subdirectory(win)
add_subdirectory(mac)
add_subdirectory(linux)
add_subdirectory(docker)
add_subdirectory(bintray)

set(MAKELIB_SCRIPT "")

if(WITH_DOUBLE_PRECISION)
    set(PD_FLOAT_SIZE 64)
else()
    set(PD_FLOAT_SIZE 32)
endif()

if(APPLE)
    set(DYLIBBUNDLER "${CMAKE_CURRENT_BINARY_DIR}/mac/dylibbundler")

    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/mac/make_lib_mac.sh
                   ${PROJECT_BINARY_DIR}/dist/make_lib_mac.sh @ONLY)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/mac/github_upload_release.sh
                   ${PROJECT_BINARY_DIR}/dist/github_upload_release.sh @ONLY)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/mac/install_external.sh
                   ${PROJECT_BINARY_DIR}/dist/install_external.sh @ONLY)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/update_cpp_src_from_doc.sh
                   ${PROJECT_BINARY_DIR}/update_cpp_src_from_doc.sh @ONLY)

    add_custom_target(ceammc_lib
        COMMAND ${CMAKE_COMMAND} -E rm -rf ${PROJECT_BINARY_DIR}/dist/pd_ceammc
        COMMAND ${PROJECT_BINARY_DIR}/dist/make_lib_mac.sh "${PROJECT_BINARY_DIR}/dist"
        SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/mac/make_lib_mac.sh)

    add_custom_target(dev_ext
        COMMAND ${PROJECT_BINARY_DIR}/dist/install_external.sh
        SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/mac/install_external.sh
        DEPENDS ceammc_lib)

    add_custom_target(dev_rmext
        COMMAND ${CMAKE_COMMAND} -E rm -rf "$ENV{HOME}/Documents/Pd/externals/ceammc")
endif()

if(UNIX AND NOT APPLE)
    set(MAKELIB_SCRIPT "${PROJECT_BINARY_DIR}/dist/make_lib_linux.sh")
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/linux/make_lib_linux.sh ${MAKELIB_SCRIPT} @ONLY)
endif()

find_program(DEKEN deken PATH $ENV{HOME}/bin)

if(DEKEN)
    message(STATUS "deken program found: ${DEKEN}. Target `make deken_package` is available")

    set(DEKEN_MAKE_PACKAGE "${PROJECT_BINARY_DIR}/dist/deken_make_package.sh")
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/deken_make_package.sh ${DEKEN_MAKE_PACKAGE} @ONLY)

    set(DEKEN_UPLOAD_PACKAGE "${PROJECT_BINARY_DIR}/dist/deken_upload_package.sh")
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/deken_upload_package.sh ${DEKEN_UPLOAD_PACKAGE} @ONLY)

    add_custom_target(deken_package
        COMMAND ${DEKEN_MAKE_PACKAGE}
        WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/dist"
        SOURCES deken_make_package.sh)

    add_custom_target(deken_upload
        COMMAND ${DEKEN_UPLOAD_PACKAGE}
        WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/dist"
        DEPENDS deken_package
        SOURCES deken_upload_package.sh)
endif()

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
set(RELEASE_TEXT "release_text_${CEAMMC_LIB_VERSION}.txt")
set(RELEASE_QLIST "release_qlist_${CEAMMC_LIB_VERSION}.txt")
set(RELEASE_TEMPLATE "template.release.pd")
set(RELEASE_PATCH "release_${CEAMMC_LIB_VERSION}.pd")
configure_file(${RELEASE_TEMPLATE} ${CMAKE_CURRENT_SOURCE_DIR}/${RELEASE_PATCH} @ONLY)
add_custom_target(release_patch
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/make_release_patch.py ${RELEASE_TEXT} > "${CMAKE_CURRENT_SOURCE_DIR}/${RELEASE_QLIST}"
    SOURCES make_release_patch.py ${RELEASE_TEXT} ${RELEASE_TEMPLATE}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

if(WIN32)
    set(DOC_INSTALL_PATH "extra/ceammc")
else()
    set(DOC_INSTALL_PATH "${PD_DOC_INSTALL_PATH}/extra/ceammc")
endif()

install(FILES ${RELEASE_QLIST} ${RELEASE_TEXT}
    DESTINATION ${DOC_INSTALL_PATH}
    PERMISSIONS OWNER_READ GROUP_READ WORLD_READ)
file(GLOB _RELEASE_FILES "release_*.pd")
foreach(f ${_RELEASE_FILES})
    install(FILES ${f}
        DESTINATION ${DOC_INSTALL_PATH}
        PERMISSIONS OWNER_READ GROUP_READ WORLD_READ)
endforeach()

