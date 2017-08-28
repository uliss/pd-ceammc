set(MIDIFILE_SOURCES
    midifile/src-library/Binasc.cpp
    midifile/src-library/MidiEvent.cpp
    midifile/src-library/MidiEventList.cpp
    midifile/src-library/MidiFile.cpp
    midifile/src-library/MidiMessage.cpp
    midifile/src-library/Options.cpp
    
    midifile/include/Binasc.h
    midifile/include/MidiEvent.h
    midifile/include/MidiEventList.h
    midifile/include/MidiFile.h
    midifile/include/MidiMessage.h
    midifile/include/Options.h
)
    
include_directories(midifile/include)
add_library(midifile STATIC ${MIDIFILE_SOURCES})

if(UNIX)
    set_target_properties(midifile PROPERTIES COMPILE_FLAGS "-fPIC")
endif()
