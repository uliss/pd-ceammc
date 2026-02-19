file(GLOB _BUILD_SCRIPTS "*.sh")
add_custom_target(ceammc_distrib_linux_sources SOURCES ${_BUILD_SCRIPTS}
                                                       rpi/pd-ceammc.desktop)

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
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/rpi/pd-ceammc.desktop
                 ${PROJECT_BINARY_DIR}/scripts/pd-ceammc.desktop @ONLY)

  if(LINUX_DEBIAN_PACKAGE_FILE_NAME)
    set(PDCORE_LIB_INSTALL_PATH lib)
    cmake_path(SET script_install_dir ${PD_DOC_INSTALL_PATH}/share/rpi)
    install(
      FILES ${PROJECT_BINARY_DIR}/scripts/pd_start.sh
      DESTINATION ${script_install_dir}
      PERMISSIONS OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ
                  WORLD_EXECUTE)

    add_custom_target(
      dev_dpkg
      COMMAND sudo dpkg --remove ${LINUX_DEBIAN_PACKAGE_NAME}
      COMMAND sudo dpkg --install
              ${PROJECT_BINARY_DIR}/${LINUX_DEBIAN_PACKAGE_FILE_NAME}.deb
      USES_TERMINAL)

    add_custom_target(
      dev_run
      COMMAND sudo cp $<TARGET_FILE:ceammc>
              ${CMAKE_INSTALL_PREFIX}/lib/pd_ceammc/extra/ceammc/
      COMMAND pd-ceammc
      DEPENDS ceammc
      USES_TERMINAL)

    add_custom_target(
      dev_open
      COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target package
      COMMAND sudo dpkg --remove ${LINUX_DEBIAN_PACKAGE_NAME}
      COMMAND sudo dpkg --install
              ${PROJECT_BINARY_DIR}/${LINUX_DEBIAN_PACKAGE_FILE_NAME}.deb
      COMMAND pd-ceammc
      USES_TERMINAL)

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
