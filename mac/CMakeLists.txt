add_custom_target(mac_deploy SOURCES
    ceammc_build.sh.in
    codesign.sh
    tcltk-libs.sh
    tcltk-wish.sh
    dmg.sh
)

if(APPLE)
    set(TCLLIB_VERSION "1.20")
    set(TKLIB_VERSION "0.6")
    set(WISH_APP_VERSION "8.6.8")

    configure_file(tcltk-libs.sh ${CMAKE_CURRENT_BINARY_DIR}/tcltk-libs.sh @ONLY)
endif()
