set(MIDI_SOURCES "")
macro(ceammc_midi_extension name)
    list(APPEND MIDI_SOURCES "midi_${name}.cpp")
endmacro()

ceammc_midi_extension(file)
include_directories(${PROJECT_SOURCE_DIR}/ceammc/extra/midifile/midifile/include)
add_library(ceammc_midi STATIC mod_midi.cpp ${MIDI_SOURCES})
if(UNIX)
    set_target_properties(ceammc_midi PROPERTIES COMPILE_FLAGS "-fPIC")
endif()

target_link_libraries(ceammc_midi midifile)
