file(GLOB _BUILD_SCRIPTS "*.sh")
add_custom_target(ceammc_distrib_linux_sources SOURCES ${_BUILD_SCRIPTS})


if(UNIX)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/snapcraft.yaml.in
               ${PROJECT_BINARY_DIR}/snap/snapcraft.yaml @ONLY)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/snapcraft.yaml.in
               ${PROJECT_SOURCE_DIR}/snap/snapcraft.yaml @ONLY)
endif()
