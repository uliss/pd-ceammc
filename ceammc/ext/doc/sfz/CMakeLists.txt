file(GLOB _PDDOC_SFZ "*.sfz")

add_custom_target(pddoc_sfz_files SOURCES
    ${_PDDOC_SFZ})

# install extra files
install(FILES
    ${_PDDOC_SFZ}
    DESTINATION ${DOC_INSTALL_PATH}/sfz
    PERMISSIONS OWNER_READ GROUP_READ WORLD_READ)
