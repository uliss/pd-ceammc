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
    endif()
endif()
