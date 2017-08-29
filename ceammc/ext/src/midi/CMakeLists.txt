set(MIDI_SOURCES 
    datatype_midistream.cpp datatype_midistream.h
    datatype_miditrack.cpp datatype_miditrack.h
)

macro(ceammc_midi_extension name)
    list(APPEND MIDI_SOURCES "midi_${name}.cpp")
endmacro()

ceammc_midi_extension(event)
ceammc_midi_extension(file)
ceammc_midi_extension(track)

include_directories(${PROJECT_SOURCE_DIR}/ceammc/extra/midifile)
add_library(ceammc_midi STATIC mod_midi.cpp ${MIDI_SOURCES})
if(UNIX)
    set_target_properties(ceammc_midi PROPERTIES COMPILE_FLAGS "-fPIC")
endif()

target_link_libraries(ceammc_midi midifile)
