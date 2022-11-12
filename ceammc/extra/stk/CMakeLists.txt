set(STK_SOURCES
    stk/src/ADSR.cpp
    stk/src/Asymp.cpp
    stk/src/BandedWG.cpp
    stk/src/BeeThree.cpp
    stk/src/BiQuad.cpp
    stk/src/Blit.cpp
    stk/src/BlitSaw.cpp
    stk/src/BlitSquare.cpp
    stk/src/BlowBotl.cpp
    stk/src/BlowHole.cpp
    stk/src/Bowed.cpp
    stk/src/Brass.cpp
    stk/src/Chorus.cpp
    stk/src/Clarinet.cpp
    stk/src/Delay.cpp
    stk/src/DelayA.cpp
    stk/src/DelayL.cpp
    stk/src/Drummer.cpp
    stk/src/Echo.cpp
    stk/src/Envelope.cpp
    stk/src/FileLoop.cpp
    stk/src/FileRead.cpp
    stk/src/FileWrite.cpp
    stk/src/FileWvIn.cpp
    stk/src/FileWvOut.cpp
    stk/src/Fir.cpp
    stk/src/Flute.cpp
    stk/src/FM.cpp
    stk/src/FMVoices.cpp
    stk/src/FormSwep.cpp
    stk/src/FreeVerb.cpp
    stk/src/Granulate.cpp
    stk/src/Guitar.cpp
    stk/src/HevyMetl.cpp
    stk/src/Iir.cpp
    stk/src/JCRev.cpp
    stk/src/LentPitShift.cpp
    stk/src/Mandolin.cpp
    stk/src/Mesh2D.cpp
    stk/src/Messager.cpp
    stk/src/MidiFileIn.cpp
    stk/src/Modal.cpp
    stk/src/ModalBar.cpp
    stk/src/Modulate.cpp
    stk/src/Moog.cpp
    stk/src/Noise.cpp
    stk/src/NRev.cpp
    stk/src/OnePole.cpp
    stk/src/OneZero.cpp
    stk/src/PercFlut.cpp
    stk/src/Phonemes.cpp
    stk/src/PitShift.cpp
    stk/src/Plucked.cpp
    stk/src/PoleZero.cpp
    stk/src/PRCRev.cpp
    stk/src/Recorder.cpp
    stk/src/Resonate.cpp
    stk/src/Rhodey.cpp
    stk/src/Sampler.cpp
    stk/src/Saxofony.cpp
    stk/src/Shakers.cpp
    stk/src/Simple.cpp
    stk/src/SineWave.cpp
    stk/src/SingWave.cpp
    stk/src/Sitar.cpp
    stk/src/Skini.cpp
    stk/src/Sphere.cpp
    stk/src/StifKarp.cpp
    stk/src/Stk.cpp
    stk/src/TapDelay.cpp
    stk/src/TubeBell.cpp
    stk/src/Twang.cpp
    stk/src/TwoPole.cpp
    stk/src/TwoZero.cpp
    stk/src/Voicer.cpp
    stk/src/VoicForm.cpp
    stk/src/Whistle.cpp
    stk/src/Wurley.cpp
)

include_directories(stk/include)
add_library(stk STATIC ${STK_SOURCES})

include(TestBigEndian)
test_big_endian(STK_IS_BIG_ENDIAN)
if(NOT STK_IS_BIG_ENDIAN)
    add_definitions(-D__LITTLE_ENDIAN__)
endif()

file(GLOB _raw_waves "stk/rawwaves/*.raw")
foreach(_w ${_raw_waves})
    install(FILES ${_w} DESTINATION ${CEAMMC_INSTALL_PATH}/stk
        PERMISSIONS OWNER_READ GROUP_READ WORLD_READ)
endforeach()

if(UNIX)
    set_target_properties(stk PROPERTIES COMPILE_FLAGS "-fPIC")
endif()

# try to read STK version
file(STRINGS "${CMAKE_CURRENT_SOURCE_DIR}/stk/configure.ac" STK_AUTOCONF_FILE LIMIT_COUNT 2)
string(REGEX MATCH "AC_INIT\\(STK,[ ]*([0-9.]+)" STK_AC_INIT ${STK_AUTOCONF_FILE})
if(STK_AC_INIT)
    set(STK_VERSION ${CMAKE_MATCH_1} CACHE INTERNAL "")
    message(STATUS "STK version: ${STK_VERSION}")
endif()

