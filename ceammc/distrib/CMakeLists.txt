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
        COMMAND ${PROJECT_BINARY_DIR}/dist/make_lib_mac.sh "${PROJECT_BINARY_DIR}/dist"
        SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/mac/make_lib_mac.sh)

    add_custom_target(install_external
        COMMAND ${PROJECT_BINARY_DIR}/dist/install_external.sh
        SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/mac/install_external.sh)
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
