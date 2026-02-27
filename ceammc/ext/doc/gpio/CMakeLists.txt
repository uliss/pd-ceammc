add_custom_target(
  dev_gen_rpi_doc
  SOURCES gen_rpi_gpio.py gen_rpi_gpio.sh
  COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/gen_rpi_gpio.sh
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  USES_TERMINAL)

install(
  FILES hw.rpi.gpio-example.pd
  DESTINATION ${DOC_INSTALL_PATH}
  PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ)
