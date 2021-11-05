set(_PDDOC_SND_LICENCE license.txt)
file(GLOB _PDDOC_SND_WAV "*.wav")
file(GLOB _PDDOC_SND_AIFF "*.aiff")
file(GLOB _PDDOC_SND_FLAC "*.flac")
file(GLOB _PDDOC_SND_MP3 "*.mp3")

add_custom_target(pddoc_snd_files SOURCES
    ${_PDDOC_SND_LICENCE}
    ${_PDDOC_SND_WAV}
    ${_PDDOC_SND_AIFF}
    ${_PDDOC_SND_FLAC}
    ${_PDDOC_SND_MP3}
)

# install extra files
install(FILES
    ${_PDDOC_SND_LICENCE}
    ${_PDDOC_SND_WAV}
    ${_PDDOC_SND_AIFF}
    ${_PDDOC_SND_FLAC}
    ${_PDDOC_SND_MP3}
    DESTINATION ${DOC_INSTALL_PATH}/sound
    PERMISSIONS OWNER_READ GROUP_READ WORLD_READ)
