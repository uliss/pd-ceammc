macro(ceammc_midi_external mod name)
    pd_add_extension(NAME "${mod}.${name}"
        FILES "${mod}_${name}.cpp"
        INTERNAL True
        LIBRARY ceammc
        LINK ceammc_core rtmidi)
endmacro()

if(APPLE AND CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_definitions(-D__MACOSX_CORE__)
    add_library(rtmidi STATIC RtMidi.cpp)
    ceammc_midi_external(midi in)
    ceammc_midi_external(midi out)
    ceammc_midi_external(midi listin)
    ceammc_midi_external(midi listout)

    pd_add_extension(NAME midi.ctlin
        FILES midiin_cc.cpp midi_in.cpp
        INTERNAL True
        LIBRARY ceammc
        LINK ceammc_core rtmidi)
endif()

