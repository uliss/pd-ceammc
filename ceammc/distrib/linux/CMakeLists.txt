file(GLOB _BUILD_SCRIPTS "*.sh")
add_custom_target(ceammc_distrib_linux_sources SOURCES ${_BUILD_SCRIPTS})


if(UNIX AND NOT APPLE)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/snapcraft.yaml.in
               ${PROJECT_BINARY_DIR}/snap/snapcraft.yaml @ONLY)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/snapcraft.yaml.in
               ${PROJECT_SOURCE_DIR}/snap/snapcraft.yaml @ONLY)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/build_faust.sh
               ${PROJECT_BINARY_DIR}/scripts/build_faust.sh @ONLY)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/build_verovio.sh
               ${PROJECT_BINARY_DIR}/scripts/build_verovio.sh @ONLY)

    if(LINUX_DEBIAN_PACKAGE_FILE_NAME)
        add_custom_target(dev_dpkg
            COMMAND sudo dpkg --remove ${LINUX_DEBIAN_PACKAGE_NAME}
            COMMAND sudo dpkg --install ${PROJECT_BINARY_DIR}/${LINUX_DEBIAN_PACKAGE_FILE_NAME}.deb
            USES_TERMINAL)

        add_custom_target(dev_run
            COMMAND sudo cp $<TARGET_FILE:ceammc> ${CMAKE_INSTALL_PREFIX}/lib/pd_ceammc/extra/ceammc/
            COMMAND pd-ceammc
            DEPENDS ceammc
            USES_TERMINAL)

        add_custom_target(dev_open
            COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target package
            COMMAND sudo dpkg --remove ${LINUX_DEBIAN_PACKAGE_NAME}
            COMMAND sudo dpkg --install ${PROJECT_BINARY_DIR}/${LINUX_DEBIAN_PACKAGE_FILE_NAME}.deb
            COMMAND pd-ceammc
            USES_TERMINAL)

        add_custom_target(dev_copyright
            COMMAND decopy
                --root ${PROJECT_SOURCE_DIR}
                --output ${PROJECT_SOURCE_DIR}/debian/copyright
                --exclude ${PROJECT_SOURCE_DIR}/build/
                --exclude ${PROJECT_SOURCE_DIR}/ceammc/distrib/
                --exclude ${PROJECT_SOURCE_DIR}/ceammc/doxygen/
                --exclude ${PROJECT_SOURCE_DIR}/ceammc/ext/benchmarks/
                --exclude ${PROJECT_SOURCE_DIR}/ceammc/ext/tests/
                --exclude ${PROJECT_SOURCE_DIR}/ceammc/extra/faust/faust/architecture/
                --exclude ${PROJECT_SOURCE_DIR}/ceammc/scripts/
                --exclude ${PROJECT_SOURCE_DIR}/m4/
                --exclude ${PROJECT_SOURCE_DIR}/mac/
                --exclude ´.*\.am´
                --exclude ´.*\.db´
                --exclude ´.*\.html?´
                --exclude ´.*\.java´
                --exclude ´.*\.json´
                --exclude ´.*\.md´
                --exclude ´.*\.pddoc´
                --exclude ´.*\.pd´
                --exclude ´.*\.sln´
                --exclude ´.*\.txt´
            USES_TERMINAL)
    endif()
endif()
