if(WIN32 OR MSYS)
    add_subdirectory(win)
endif()

if(APPLE)
    add_subdirectory(mac)
endif()

set(MAKELIB_SCRIPT "")
set(MAKELIB_COMPAT_SCRIPT "")

if(APPLE)
    set(DYLIBBUNDLER "${CMAKE_CURRENT_BINARY_DIR}/mac/dylibbundler")
    set(MAKELIB_SCRIPT "${CMAKE_CURRENT_BINARY_DIR}/make_lib_mac.sh")
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/make_lib_mac.sh
                   ${CMAKE_CURRENT_BINARY_DIR}/make_lib_mac.sh @ONLY)

    set(MAKELIB_COMPAT_SCRIPT "${CMAKE_CURRENT_BINARY_DIR}/make_lib_compat_mac.sh")
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/make_lib_compat_mac.sh
                   ${CMAKE_CURRENT_BINARY_DIR}/make_lib_compat_mac.sh @ONLY)

endif()

if(MAKELIB_SCRIPT)
    add_custom_target(ceammc_lib
        COMMAND "${MAKELIB_SCRIPT}"
            "${CMAKE_CURRENT_SOURCE_DIR}/.."
            "${CMAKE_CURRENT_BINARY_DIR}/.."
            "${CMAKE_BINARY_DIR}/dist"
            "${CEAMMC_LIB_VERSION}"
        SOURCES make_lib_mac.sh)
endif()

if(MAKELIB_COMPAT_SCRIPT)
    add_custom_target(ceammc_lib_compat
        COMMAND "${MAKELIB_COMPAT_SCRIPT}"
            "${CMAKE_CURRENT_SOURCE_DIR}/.."
            "${CMAKE_CURRENT_BINARY_DIR}/.."
            "${CMAKE_BINARY_DIR}/dist"
            "${CEAMMC_LIB_VERSION}"
        SOURCES make_lib_compat_mac.sh)
endif()

add_custom_target(distrib_sources SOURCES ceammc_pddoc_link_fix.py)
