set(INSTALLED_BIN_DIR "${CMAKE_INSTALL_PREFIX}")

if(WIN32)
    find_program(7Z_EXE
        NAMES "7z" "7z.exe"
        PATH_SUFFIXES "7-Zip"
    )
    if(7Z_EXE)
        message(STATUS "7z found at: ${7Z_EXE}")
    endif()

    find_program(GITHUB_CLI NAMES gh gh.exe)
    if(GITHUB_CLI)
        message(STATUS "GitHub command line found: ${GITHUB_CLI}")
    endif()

    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(CEAMMC_DISTRIB_ARCH "x86_64")
    elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
        set(CEAMMC_DISTRIB_ARCH "i686")
    endif()

    if(WITH_DOUBLE_PRECISION)
        set(CEAMMC_PRECISION "double")
        set(CEAMMC_APP_SUFFIX " (double)")
        set(PD_FLOAT_SIZE 64)
    else()
        set(CEAMMC_PRECISION "float")
        set(PD_FLOAT_SIZE 32)
    endif()

    set(INNO_INSTALLER_NAME "PureData-ceammc-${CEAMMC_DISTRIB_VERSION}-${CEAMMC_DISTRIB_ARCH}-${CEAMMC_PRECISION}-setup")

    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/pd.iss
                   ${PROJECT_BINARY_DIR}/dist/pd.iss)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/fix_dll_deps.sh
                   ${PROJECT_BINARY_DIR} @ONLY)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/win_fix_pddoc_key_modifiers.sh
                   ${PROJECT_BINARY_DIR}/dist/win_fix_pddoc_key_modifiers.sh @ONLY)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/make_lib_win.sh
                   ${PROJECT_BINARY_DIR}/dist/make_lib_win.sh @ONLY)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/install_external.sh
                   ${PROJECT_BINARY_DIR}/dist/install_external.sh @ONLY)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/remove_external.sh
                   ${PROJECT_BINARY_DIR}/dist/remove_external.sh @ONLY)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/build_all.sh
                   ${PROJECT_BINARY_DIR}/dist/build_all.sh @ONLY)

   add_custom_target(dev_fixdll
       COMMAND bash -c "${PROJECT_BINARY_DIR}/fix_dll_deps.sh"
       USES_TERMINAL)

   add_custom_target(dev_fixdoc
       COMMAND bash -c "${PROJECT_BINARY_DIR}/dist/win_fix_pddoc_key_modifiers.sh"
       USES_TERMINAL)

    add_custom_target(ceammc_lib
        COMMAND bash -c "${PROJECT_BINARY_DIR}/dist/make_lib_win.sh '${PROJECT_BINARY_DIR}/dist'"
        USES_TERMINAL)

    add_custom_target(dev_ext
        COMMAND bash -c "${PROJECT_BINARY_DIR}/dist/install_external.sh"
        USES_TERMINAL)

    add_custom_target(dev_rmext
        COMMAND bash -c "${PROJECT_BINARY_DIR}/dist/remove_external.sh")

    add_custom_target(dev_build_all
        COMMAND bash -c "${PROJECT_BINARY_DIR}/dist/build_all.sh"
        USES_TERMINAL)

    # deploy to github
    # deploy INNO installer
    set(GIT_RELEASE_TAG "v${CEAMMC_DISTRIB_VERSION}")
    add_custom_target(deploy_github_exe
        COMMAND ${GITHUB_CLI} release upload --clobber ${GIT_RELEASE_TAG} "${PROJECT_BINARY_DIR}/dist/${INNO_INSTALLER_NAME}.exe"
        USES_TERMINAL)
    # deploy external
    add_custom_target(deploy_github_ext
        COMMAND ${GITHUB_CLI} release upload --clobber ${GIT_RELEASE_TAG} "${PROJECT_BINARY_DIR}/dist/${CEAMMC_EXTERNAL_NAME}"
        USES_TERMINAL)
    add_custom_target(deploy_github DEPENDS deploy_github_exe deploy_github_ext)

    add_custom_target(install0
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:ceammc_core> "${CMAKE_INSTALL_PREFIX}/${PD_INTERNAL_EXT_INSTALL_PATH}/ceammc"
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:ceammc> "${CMAKE_INSTALL_PREFIX}/${PD_INTERNAL_EXT_INSTALL_PATH}/ceammc")
    add_dependencies(install0 ceammc ceammc_core)

    add_custom_target(run0 COMMAND "${CMAKE_INSTALL_PREFIX}/${PD_EXE_INSTALL_PATH}/$<TARGET_FILE_NAME:pd>")
    add_dependencies(run0 install0)
    add_custom_target(run1 COMMAND "${CMAKE_INSTALL_PREFIX}/${PD_EXE_INSTALL_PATH}/$<TARGET_FILE_NAME:pd>" -d 1)
    add_dependencies(run1 install0)
    add_custom_target(run2 COMMAND "${CMAKE_INSTALL_PREFIX}/${PD_EXE_INSTALL_PATH}/$<TARGET_FILE_NAME:pd>" -d 2)
    add_dependencies(run2 install0)
    add_custom_target(run3 COMMAND "${CMAKE_INSTALL_PREFIX}/${PD_EXE_INSTALL_PATH}/$<TARGET_FILE_NAME:pd>" -d 3)
    add_dependencies(run3 install0)
endif()


if(WIN32 OR MSYS OR MINGW)
    install(FILES fonts/DejaVuSansMono.ttf
        DESTINATION ${PD_FONT_INSTALL_PATH})
endif()

find_program(ISCC_PATH
    NAMES ISCC.exe
    PATHS "$ENV{PROGRAMFILES}/Inno Setup 5"
          "C:/Program Files (x86)/Inno Setup 5"
          "$ENV{PROGRAMFILES}/Inno Setup 6"
          "C:/Program Files (x86)/Inno Setup 6")


if(ISCC_PATH)
    message(STATUS "Inno Setup Installed found: ${ISCC_PATH}")
    message(STATUS "     You could type `make inno` after `make install` to build installer package")
    add_custom_target(inno
        COMMAND ${ISCC_PATH} "${PROJECT_BINARY_DIR}/dist/pd.iss"
        USES_TERMINAL)
endif()

add_subdirectory(homebrew)

if(WIN32)
    install(DIRECTORY fonts DESTINATION ${CMAKE_INSTALL_PREFIX})
endif()

add_custom_target(ceammc_distrib_win_sources SOURCES
    fix_dll_deps.sh
    make_lib_win.sh
    win_fix_pddoc_key_modifiers.sh
    install_external.sh
    remove_external.sh
    build_all.sh
    pd.iss
)
