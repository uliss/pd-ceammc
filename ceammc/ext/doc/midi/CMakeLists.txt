file(GLOB _PDDOC_MIDI "*.mid")

add_custom_target(pddoc_midi_files SOURCES
    ${_PDDOC_MIDI})

# install extra files
install(FILES
    ${_PDDOC_MIDI}
    DESTINATION ${DOC_INSTALL_PATH}/midi
    PERMISSIONS OWNER_READ GROUP_READ WORLD_READ)
