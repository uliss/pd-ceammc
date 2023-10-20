file(GLOB _WAV_FILES "*.wav")

install(
    FILES "LICENSE.md" ${_WAV_FILES}
    DESTINATION ${CEAMMC_INSTALL_PATH}/ir)
