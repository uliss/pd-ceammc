add_custom_target(mac_deploy SOURCES
    ceammc_build.sh.in
    codesign.sh
    tcltk-wish.sh
    dmg.sh
)
