if(APPLE)
    add_executable(dylibbundler
        macdylibbundler/src/Dependency.cpp
        macdylibbundler/src/DylibBundler.cpp
        macdylibbundler/src/main.cpp
        macdylibbundler/src/Settings.cpp
        macdylibbundler/src/Utils.cpp)

    file(GLOB _BUILD_SCRIPTS "*.sh")
    add_custom_target(ceammc_distrib_mac_sources SOURCES ${_BUILD_SCRIPTS})

    if(WITH_DOUBLE_PRECISION)
        set(PD_FLOAT_SIZE 64)
    else()
        set(PD_FLOAT_SIZE 32)
    endif()

    set(DYLIBBUNDLER ${CMAKE_CURRENT_BINARY_DIR}/dylibbundler)
    set(CEAMMC_EXTERNAL_FULL_PATH "${PROJECT_BINARY_DIR}/dist/${CEAMMC_EXTERNAL_NAME}")

    set(BUNDLE "Pd-${PD_MACOSX_BUNDLE_SUFFIX}.app")
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

    add_custom_target(github_release_ext
        COMMAND ${PROJECT_BINARY_DIR}/dist/github_upload_release.sh ${CEAMMC_EXTERNAL_FULL_PATH})


    find_program(GPGENV_EXE NAMES gpgenv)

    if(GPGENV_EXE)
        add_custom_target(deploy
            COMMAND
                ${PROJECT_BINARY_DIR}/dist/github_upload_release.sh
                "${PROJECT_BINARY_DIR}/Pd_${PD_MACOSX_BUNDLE_SUFFIX}-macosx-${MACOSX_VERSION}.dmg"
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
            DEPENDS dmg src-zip src-tar ceammc_lib)
    endif()
endif()
