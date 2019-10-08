add_subdirectory(win)
add_subdirectory(mac)
add_subdirectory(linux)
add_subdirectory(docker)

set(MAKELIB_SCRIPT "")

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(CEAMMC_DISTRIB_ARCH "64-bit")
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
    set(CEAMMC_DISTRIB_ARCH "32-bit")
endif()

if(APPLE)
    set(DYLIBBUNDLER "${CMAKE_CURRENT_BINARY_DIR}/mac/dylibbundler")
    set(MAKELIB_SCRIPT "${PROJECT_BINARY_DIR}/dist/make_lib_mac.sh")
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/mac/make_lib_mac.sh
                   ${MAKELIB_SCRIPT} @ONLY)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/mac/github_upload_release.sh
                   ${PROJECT_BINARY_DIR}/dist/github_upload_release.sh @ONLY)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/mac/install_external.sh
                   ${PROJECT_BINARY_DIR}/dist/install_external.sh @ONLY)
endif()

if(UNIX AND NOT APPLE)
    set(MAKELIB_SCRIPT "${PROJECT_BINARY_DIR}/dist/make_lib_linux.sh")
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/linux/make_lib_linux.sh ${MAKELIB_SCRIPT} @ONLY)
endif()

if(WIN32 OR MSYS)
    find_program(7Z_EXE NAMES "7z" "7z.exe")
    if(7Z_EXE)
        message(STATUS "7z found at: ${7Z_EXE}")
    endif()
    set(MAKELIB_SCRIPT "${PROJECT_BINARY_DIR}/dist/make_lib_win.sh")
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/win/make_lib_win.sh ${MAKELIB_SCRIPT} @ONLY)
endif()

if(MAKELIB_SCRIPT)
    if(APPLE)
        add_custom_target(ceammc_lib
            COMMAND "${MAKELIB_SCRIPT}"
                "${CMAKE_CURRENT_SOURCE_DIR}/.."
                "${CMAKE_CURRENT_BINARY_DIR}/../ext"
                "${CMAKE_BINARY_DIR}/dist"
            SOURCES mac/make_lib_mac.sh)

        add_custom_target(install_external COMMAND ${PROJECT_BINARY_DIR}/dist/install_external.sh)
    elseif(WIN32)
        add_custom_target(ceammc_lib
            COMMAND "${MAKELIB_SCRIPT}"
                "${CMAKE_BINARY_DIR}/dist"
            SOURCES win/make_lib_win.sh)
    endif()
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
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/doc_check_methods.sh ${PROJECT_BINARY_DIR} @ONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/check_all_pddoc_refs.sh ${PROJECT_BINARY_DIR} @ONLY)

add_custom_target(ceammc_doc_check_methods
    COMMAND ${PROJECT_BINARY_DIR}/doc_check_methods.sh
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR})
