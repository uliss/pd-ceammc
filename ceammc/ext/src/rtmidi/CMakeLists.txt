macro(ceammc_midi_external name)
    pd_add_extension(NAME "midi.${name}"
        FILES "midi_${name}.cpp"
        INTERNAL True
        LIBRARY ceammc
        LINK ceammc_core rtmidi)
endmacro()

if(APPLE AND CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_definitions(-D__MACOSX_CORE__)
    add_library(rtmidi STATIC RtMidi.cpp)
    ceammc_midi_external(in)
    ceammc_midi_external(out)
    ceammc_midi_external(listin)
    ceammc_midi_external(listout)
endif()

