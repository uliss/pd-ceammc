add_subdirectory(win)
add_subdirectory(mac)
add_subdirectory(linux)
add_subdirectory(docker)

set(MAKELIB_SCRIPT "")

if(APPLE)
    set(DYLIBBUNDLER "${CMAKE_CURRENT_BINARY_DIR}/mac/dylibbundler")
    set(MAKELIB_SCRIPT "${CMAKE_CURRENT_BINARY_DIR}/mac/make_lib_mac.sh")
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/mac/make_lib_mac.sh
                   ${CMAKE_CURRENT_BINARY_DIR}/mac/make_lib_mac.sh @ONLY)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/mac/github_upload_release.sh
                   ${PROJECT_BINARY_DIR}/dist/github_upload_release.sh @ONLY)
endif()

if(UNIX AND NOT APPLE)
    set(MAKELIB_SCRIPT "${PROJECT_BINARY_DIR}/dist/make_lib_linux.sh")
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/linux/make_lib_linux.sh ${MAKELIB_SCRIPT} @ONLY)
endif()

if(WIN32 OR MSYS)
    set(MAKELIB_SCRIPT "${PROJECT_BINARY_DIR}/dist/make_lib_win.sh")
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/win/make_lib_win.sh ${MAKELIB_SCRIPT} @ONLY)
endif()

if(MAKELIB_SCRIPT)
    add_custom_target(ceammc_lib
        COMMAND "${MAKELIB_SCRIPT}"
            "${CMAKE_CURRENT_SOURCE_DIR}/.."
            "${CMAKE_CURRENT_BINARY_DIR}/../ext"
            "${CMAKE_BINARY_DIR}/dist"
            "${CEAMMC_LIB_VERSION}"
        SOURCES mac/make_lib_mac.sh)
endif()

find_program(DEKEN deken PATH $ENV{HOME}/bin)

if(DEKEN)
    message(STATUS "deken program found: ${DEKEN}. Target `make deken` is available")

    set(DEKEN_MAKE_PACKAGE "${PROJECT_BINARY_DIR}/dist/deken_make_package.sh")
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/deken_make_package.sh ${DEKEN_MAKE_PACKAGE} @ONLY)

    add_custom_target(deken
        COMMAND ${DEKEN_MAKE_PACKAGE}
        WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/dist"
        DEPENDS ceammc_lib
        SOURCES deken_make_package.sh)
endif()

# documentation check
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/list_all_doc_externals.sh ${PROJECT_BINARY_DIR} @ONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/check_doc_external.py ${PROJECT_BINARY_DIR} @ONLY)
