set(MIDI_SOURCES 
    datatype_midistream.cpp datatype_midistream.h
    datatype_miditrack.cpp datatype_miditrack.h
    midi_common.cpp midi_common.h
    property_pitch.cpp
)

macro(ceammc_midi_external name)
    list(APPEND MIDI_SOURCES "midi_${name}.cpp")
endmacro()

ceammc_midi_external(arp)
ceammc_midi_external(cc)
ceammc_midi_external(clock)
ceammc_midi_external(ctl2str)
ceammc_midi_external(event)
ceammc_midi_external(event_to_ctl)
ceammc_midi_external(file)
ceammc_midi_external(kbd)
ceammc_midi_external(key2str)
ceammc_midi_external(modus)
ceammc_midi_external(octave)
ceammc_midi_external(prg2str)
ceammc_midi_external(split)
ceammc_midi_external(str2key)
ceammc_midi_external(sustain)
ceammc_midi_external(sysex)
ceammc_midi_external(track)
ceammc_midi_external(tuning)
ceammc_midi_external(velocity_ramp)
ceammc_midi_external(vrand)

add_library(ceammc_midi STATIC mod_midi.cpp ${MIDI_SOURCES})
target_include_directories(ceammc_midi
    PRIVATE
        ${PROJECT_SOURCE_DIR}/ceammc/extra/midifile
        "$<TARGET_PROPERTY:fmt,INCLUDE_DIRECTORIES>"
)

target_link_libraries(ceammc_midi midifile)

file(GLOB _MIDI_SRC_FILES "kbd_*.txt" "*.py")
add_custom_target(ceammc_midi_src SOURCES ${_MIDI_SRC_FILES})
