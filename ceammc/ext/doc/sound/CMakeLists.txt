set(_PDDOC_SND_LICENCE license.txt)
file(GLOB _PDDOC_SND_WAV "*.wav")
file(GLOB _PDDOC_SND_AIFF "*.aiff")

add_custom_target(pddoc_snd_files SOURCES
    ${_PDDOC_SND_LICENCE}
    ${_PDDOC_SND_WAV}
    ${_PDDOC_SND_AIFF})

# install extra files
install(FILES
    ${_PDDOC_SND_LICENCE}
    ${_PDDOC_SND_WAV}
    ${_PDDOC_SND_AIFF}
    DESTINATION ${DOC_INSTALL_PATH}/sound
    PERMISSIONS OWNER_READ GROUP_READ WORLD_READ)
