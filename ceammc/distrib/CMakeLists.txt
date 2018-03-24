add_subdirectory(win)
add_subdirectory(mac)
add_subdirectory(linux)

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
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/linux/make_lib_linux.sh
                   ${MAKELIB_SCRIPT} @ONLY)
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
