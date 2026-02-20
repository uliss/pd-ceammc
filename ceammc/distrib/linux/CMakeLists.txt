file(GLOB _BUILD_SCRIPTS "*.sh")
file(GLOB _RPI_EXAMPLES "rpi/examples/*.pd")
add_custom_target(
  ceammc_distrib_linux_sources SOURCES ${_BUILD_SCRIPTS} ${_RPI_EXAMPLES}
                                       rpi/pd-ceammc-autostart.desktop)

if(UNIX AND NOT APPLE)
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/snapcraft.yaml.in
                 ${PROJECT_BINARY_DIR}/snap/snapcraft.yaml @ONLY)
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/snapcraft.yaml.in
                 ${PROJECT_SOURCE_DIR}/snap/snapcraft.yaml @ONLY)
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/build_faust.sh
                 ${PROJECT_BINARY_DIR}/scripts/build_faust.sh @ONLY)
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/build_verovio.sh
                 ${PROJECT_BINARY_DIR}/scripts/build_verovio.sh @ONLY)

  # Raspberry Pi run script, supposed to be installed into ${HOME}/bin
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/rpi/pd_start.sh
                 ${PROJECT_BINARY_DIR}/scripts/pd_start.sh @ONLY)

  if(LINUX_DEBIAN_PACKAGE_FILE_NAME)
    set(PDCORE_LIB_INSTALL_PATH lib)

    # RaspberryPi
    # ##########################################################################
    # extra stuff install directory
    cmake_path(SET rpi_script_install_dir ${PD_DOC_INSTALL_PATH}/share/rpi)
    # install run_script for autostart
    install(
      FILES ${PROJECT_BINARY_DIR}/scripts/pd_start.sh
      DESTINATION ${rpi_script_install_dir}
      PERMISSIONS
        OWNER_READ
        OWNER_WRITE
        OWNER_EXECUTE
        GROUP_READ
        GROUP_EXECUTE
        WORLD_READ
        WORLD_EXECUTE)
    # install RPi desktop file for autostart
    install(
      FILES ${CMAKE_CURRENT_SOURCE_DIR}/rpi/pd-ceammc-autostart.desktop
      DESTINATION ${rpi_script_install_dir}
      PERMISSIONS
        OWNER_READ
        OWNER_WRITE
        OWNER_EXECUTE
        GROUP_READ
        GROUP_EXECUTE
        WORLD_READ
        WORLD_EXECUTE)
    # copy examples
    install(
      DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/rpi/examples
      DESTINATION ${rpi_script_install_dir}/
      FILE_PERMISSIONS
        OWNER_READ
        OWNER_WRITE
        GROUP_READ
        WORLD_READ)

    # Linux dev targets
    # ##########################################################################
    # (re)install with dpkg (slow!)
    add_custom_target(
      dev_dpkg
      COMMAND sudo dpkg --purge ${LINUX_DEBIAN_PACKAGE_NAME}
      COMMAND sudo dpkg --install
              ${PROJECT_BINARY_DIR}/${LINUX_DEBIAN_PACKAGE_FILE_NAME}.deb
      USES_TERMINAL)

    # rebuild all (re)install with dpkg (slow!)
    add_custom_target(
      dev_package
      COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target package
      COMMAND sudo dpkg --purge ${LINUX_DEBIAN_PACKAGE_NAME}
      COMMAND sudo dpkg --install
              ${PROJECT_BINARY_DIR}/${LINUX_DEBIAN_PACKAGE_FILE_NAME}.deb
      USES_TERMINAL)

    # fast run: copy binary and run (be carefull!)
    add_custom_target(
      dev_run
      COMMAND sudo cp $<TARGET_FILE:ceammc>
              ${CMAKE_INSTALL_PREFIX}/lib/pd_ceammc/extra/ceammc/
      COMMAND pd-ceammc
      DEPENDS ceammc
      USES_TERMINAL)

    # slow run: build app, (re)install with dpkg, then run
    add_custom_target(
      dev_open
      COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target package
      COMMAND sudo dpkg --remove ${LINUX_DEBIAN_PACKAGE_NAME}
      COMMAND sudo dpkg --install
              ${PROJECT_BINARY_DIR}/${LINUX_DEBIAN_PACKAGE_FILE_NAME}.deb
      COMMAND pd-ceammc
      USES_TERMINAL)

    # update copyrights
    add_custom_target(
      dev_copyright
      COMMAND
        decopy --root ${PROJECT_SOURCE_DIR} --output
        ${PROJECT_SOURCE_DIR}/debian/copyright --exclude
        ${PROJECT_SOURCE_DIR}/build/ --exclude
        ${PROJECT_SOURCE_DIR}/ceammc/distrib/ --exclude
        ${PROJECT_SOURCE_DIR}/ceammc/doxygen/ --exclude
        ${PROJECT_SOURCE_DIR}/ceammc/ext/benchmarks/ --exclude
        ${PROJECT_SOURCE_DIR}/ceammc/ext/tests/ --exclude
        ${PROJECT_SOURCE_DIR}/ceammc/extra/faust/faust/architecture/ --exclude
        ${PROJECT_SOURCE_DIR}/ceammc/scripts/ --exclude
        ${PROJECT_SOURCE_DIR}/m4/ --exclude ${PROJECT_SOURCE_DIR}/mac/ --exclude
        ´.* \.am´ --exclude ´.* \.db´ --exclude ´.* \.html?´ --exclude ´.*
        \.java´ --exclude ´.* \.json´ --exclude ´.* \.md´ --exclude ´.* \.pddoc´
        --exclude ´.* \.pd´ --exclude ´.* \.sln´ --exclude ´.* \.txt´
      USES_TERMINAL)
  endif()
endif()
