include(library_pddoc.cmake)

add_subdirectory(CicmWrapper)
add_subdirectory(CreamLibrary)
add_subdirectory(hcs)
add_subdirectory(comport)
add_subdirectory(SoundTouch)

add_custom_target(_ceammc_extra_misc SOURCES extra_objects.txt)
