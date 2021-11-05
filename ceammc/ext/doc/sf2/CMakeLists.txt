file(GLOB _PDDOC_SF2 "*.sf2")

add_custom_target(pddoc_sf2_files SOURCES
    ${_PDDOC_SF2})

# install extra files
install(FILES
    ${_PDDOC_SF2}
    DESTINATION ${DOC_INSTALL_PATH}/sf2
    PERMISSIONS OWNER_READ GROUP_READ WORLD_READ)
