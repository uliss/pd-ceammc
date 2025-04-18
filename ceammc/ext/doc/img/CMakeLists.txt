file(GLOB _PNG_FILES "*.png")
file(GLOB _JPG_FILES "*.jpg")

set (SVG_FILES
    2d_matrix.svg
    sample.svg
)

install(
    FILES ${SVG_FILES} ${_PNG_FILES} ${_JPG_FILES}
    DESTINATION ${CEAMMC_INSTALL_PATH}/img
)
