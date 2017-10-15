set(MIDIFILE_SOURCES
    Binasc.cpp
    MidiEvent.cpp
    MidiEventList.cpp
    MidiFile.cpp
    MidiMessage.cpp
    
    Binasc.h
    MidiEvent.h
    MidiEventList.h
    MidiFile.h
    MidiMessage.h
)
    
add_library(midifile STATIC ${MIDIFILE_SOURCES})

if(UNIX)
    set_target_properties(midifile PROPERTIES COMPILE_FLAGS "-fPIC")
endif()
