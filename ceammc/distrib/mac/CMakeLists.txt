file(GLOB _BUILD_SCRIPTS "*.sh")
add_custom_target(ceammc_distrib_mac_sources SOURCES ${_BUILD_SCRIPTS})

if(APPLE)
    add_executable(dylibbundler
        macdylibbundler/src/Dependency.cpp
        macdylibbundler/src/DylibBundler.cpp
        macdylibbundler/src/main.cpp
        macdylibbundler/src/Settings.cpp
        macdylibbundler/src/Utils.cpp)

    if(WITH_DOUBLE_PRECISION)
        set(PD_FLOAT_SIZE 64)
    else()
        set(PD_FLOAT_SIZE 32)
    endif()

    set(DYLIBBUNDLER ${CMAKE_CURRENT_BINARY_DIR}/dylibbundler)
    set(CEAMMC_EXTERNAL_FULL_PATH "${PROJECT_BINARY_DIR}/dist/${CEAMMC_EXTERNAL_NAME}")

    set(BUNDLE ${PD_MACOSX_APP})
    set(BUNDLE_FULL_PATH "${PROJECT_BINARY_DIR}/dist/${BUNDLE}")
    set(PD_BUNDLE_FULL_PATH ${BUNDLE_FULL_PATH}/Contents/Resources/bin/pd)

    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/make_lib_mac.sh
                   ${PROJECT_BINARY_DIR}/dist/make_lib_mac.sh @ONLY)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/github_upload_release.sh
                   ${PROJECT_BINARY_DIR}/dist/github_upload_release.sh @ONLY)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/install_external.sh
                   ${PROJECT_BINARY_DIR}/dist/install_external.sh @ONLY)

    configure_file(gen_examples.sh ${PROJECT_BINARY_DIR}/scripts/gen_examples.sh @ONLY)
    configure_file(ps_to_png.sh ${PROJECT_BINARY_DIR}/scripts/ps_to_png.sh @ONLY)
    configure_file(save_postscript.sh ${PROJECT_BINARY_DIR}/scripts/save_postscript.sh @ONLY)
    configure_file(save_example.sh ${PROJECT_BINARY_DIR}/scripts/save_example.sh @ONLY)
    configure_file(wish_activate.sh ${PROJECT_BINARY_DIR}/scripts/wish_activate.sh @ONLY)


    find_program(GITHUB_CLI NAMES gh gh.exe)
    if(GITHUB_CLI)
        message(STATUS "GitHub command line found: ${GITHUB_CLI}")

        set(GIT_RELEASE_TAG "v${CEAMMC_DISTRIB_VERSION}")

        add_custom_target(deploy_github_ext
            COMMAND ${GITHUB_CLI} release upload --clobber ${GIT_RELEASE_TAG} ${CEAMMC_EXTERNAL_FULL_PATH}
            USES_TERMINAL)

        add_custom_target(deploy_github_appzip
            COMMAND ${GITHUB_CLI} release upload --clobber ${GIT_RELEASE_TAG} ${PROJECT_BINARY_DIR}/dist/${PD_MACOSX_ZIP}
            USES_TERMINAL)

        add_custom_target(deploy_github_dmg
            COMMAND ${GITHUB_CLI} release upload --clobber ${GIT_RELEASE_TAG} ${PROJECT_BINARY_DIR}/dist/${PD_MACOSX_DMG}
            USES_TERMINAL)

        add_custom_target(deploy_github DEPENDS deploy_github_appzip deploy_github_dmg deploy_github_ext)
    endif()

    add_custom_target(dev_build_all
        DEPENDS dev_touch_version all ceammc_lib dmg)
endif()
