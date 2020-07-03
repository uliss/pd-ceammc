include(PdExternal)
add_definitions(-DPD)
add_subdirectory(bob~)
add_subdirectory(bonk~)
add_subdirectory(choice)
add_subdirectory(fiddle~)
add_subdirectory(loop~)
add_subdirectory(lrshift~)
add_subdirectory(pd~)
add_subdirectory(pique)
add_subdirectory(sigmund~)
add_subdirectory(stdout)

# install vanilla abstractions
install(FILES
    complex-mod~-help.pd
    complex-mod~.pd
    hilbert~-help.pd
    hilbert~.pd
    rev1-final.pd
    rev1-stage.pd
    rev1~-help.pd
    rev1~.pd
    rev2~-help.pd
    rev2~.pd
    rev3~-help.pd
    rev3~.pd
    DESTINATION ${PD_INTERNAL_EXT_INSTALL_PATH}
    PERMISSIONS OWNER_READ GROUP_READ WORLD_READ)
