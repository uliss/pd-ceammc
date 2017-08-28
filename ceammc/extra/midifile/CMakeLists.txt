if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR CMAKE_COMPILER_IS_GNUCXX)
    include(CheckCXXCompilerFlag)
    check_cxx_compiler_flag(--std=c++11 SUPPORTS_STD_CXX11)
    if(SUPPORTS_STD_CXX11)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --std=c++11")
        message(STATUS "Building with midifile support")
    else()
        message(STATUS "Compiler does not support --std=c++11. Midifile skipped.")
    endif()
endif()

if(SUPPORTS_STD_CXX11)
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
endif()
