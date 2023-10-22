file(GLOB _SVG_FILES "*.svg")
file(GLOB _PNG_FILES "*.png")
file(GLOB _JPG_FILES "*.jpg")

install(
    FILES ${_SVG_FILES} ${_PNG_FILES} ${_JPG_FILES}
    DESTINATION ${CEAMMC_INSTALL_PATH}/img
)
