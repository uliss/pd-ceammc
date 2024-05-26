file(GLOB _BUILD_SCRIPTS "*.sh")
add_custom_target(ceammc_distrib_linux_sources SOURCES ${_BUILD_SCRIPTS})


if(UNIX AND NOT APPLE)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/snapcraft.yaml.in
               ${PROJECT_BINARY_DIR}/snap/snapcraft.yaml @ONLY)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/snapcraft.yaml.in
               ${PROJECT_SOURCE_DIR}/snap/snapcraft.yaml @ONLY)

    if(LINUX_DEBIAN_PACKAGE_FILE_NAME)
        add_custom_target(dev_dpkg
            COMMAND sudo dpkg --remove ${LINUX_DEBIAN_PACKAGE_NAME}
            COMMAND sudo dpkg --install ${PROJECT_BINARY_DIR}/${LINUX_DEBIAN_PACKAGE_FILE_NAME}
            USES_TERMINAL)

        add_custom_target(dev_copyright
            COMMAND decopy
                --root ${PROJECT_SOURCE_DIR}
                --output ${PROJECT_SOURCE_DIR}/debian/copyright
                --exclude ${PROJECT_SOURCE_DIR}/build
                --exclude ${PROJECT_SOURCE_DIR}/m4
                --exclude ${PROJECT_SOURCE_DIR}/mac
                --exclude ${PROJECT_SOURCE_DIR}/ceammc/doxygen
                --exclude ${PROJECT_SOURCE_DIR}/ceammc/distrib
                --exclude ${PROJECT_SOURCE_DIR}/ceammc/scripts
                --exclude ${PROJECT_SOURCE_DIR}/ceammc/ext/tests
                --exclude ${PROJECT_SOURCE_DIR}/ceammc/ext/benchmarks
                --exclude '.*\.json'
                --exclude '.*\.pd'
                --exclude '.*\.pddoc'
                --exclude '.*\.html?'
                --exclude '.*\.txt'
                --exclude '.*\.md'
                --exclude '.*\.java'
                --exclude ${PROJECT_SOURCE_DIR}/ceammc/extra/faust/faust/architecture/
            USES_TERMINAL)
    endif()
endif()
