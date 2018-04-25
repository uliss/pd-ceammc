include(library_pddoc.cmake)

add_subdirectory(hcs)
add_subdirectory(comport)
add_subdirectory(SoundTouch)
add_subdirectory(flext)
add_subdirectory(libdir)
add_subdirectory(import)
add_subdirectory(lyonpotpourri)
add_subdirectory(fftease)
add_subdirectory(autotune)
add_subdirectory(stk)
add_subdirectory(midifile)
add_subdirectory(serial)
add_subdirectory(speech)
add_subdirectory(fluidsynth)


set(EXTRA_COMPLETION "extra_objects.txt")
add_custom_target(_ceammc_extra_misc SOURCES ${EXTRA_COMPLETION})
install(FILES ${EXTRA_COMPLETION} DESTINATION ${PD_GUI_PLUGINS_PATH}/ceammc)

