if(APPLE)
    add_executable(dylibbundler
        macdylibbundler/src/Dependency.cpp
        macdylibbundler/src/DylibBundler.cpp
        macdylibbundler/src/main.cpp
        macdylibbundler/src/Settings.cpp
        macdylibbundler/src/Utils.cpp)

    file(GLOB _BUILD_SCRIPTS "*.sh")
    add_custom_target(ceammc_distrib_mac_sources SOURCES ${_BUILD_SCRIPTS})

    set(BUNDLE "Pd-${PD_MACOSX_BUNDLE_SUFFIX}.app")
    set(BUNDLE_FULL_PATH "${PROJECT_BINARY_DIR}/dist/${BUNDLE}")
    set(PD_BUNDLE_FULL_PATH ${BUNDLE_FULL_PATH}/Contents/Resources/bin/pd)

    configure_file(gen_examples.sh ${PROJECT_BINARY_DIR}/scripts/gen_examples.sh @ONLY)
    configure_file(ps_to_png.sh ${PROJECT_BINARY_DIR}/scripts/ps_to_png.sh @ONLY)
    configure_file(save_postscript.sh ${PROJECT_BINARY_DIR}/scripts/save_postscript.sh @ONLY)
    configure_file(save_example.sh ${PROJECT_BINARY_DIR}/scripts/save_example.sh @ONLY)
    configure_file(wish_activate.sh ${PROJECT_BINARY_DIR}/scripts/wish_activate.sh @ONLY)
endif()
