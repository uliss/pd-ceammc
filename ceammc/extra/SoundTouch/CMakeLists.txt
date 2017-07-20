set(SOUNDTOUCH_SRC
    soundtouch/source/SoundTouch/AAFilter.cpp
    soundtouch/source/SoundTouch/BPMDetect.cpp
    soundtouch/source/SoundTouch/FIFOSampleBuffer.cpp
    soundtouch/source/SoundTouch/FIRFilter.cpp
    soundtouch/source/SoundTouch/InterpolateCubic.cpp
    soundtouch/source/SoundTouch/InterpolateLinear.cpp
    soundtouch/source/SoundTouch/InterpolateShannon.cpp
    soundtouch/source/SoundTouch/PeakFinder.cpp
    soundtouch/source/SoundTouch/RateTransposer.cpp
    soundtouch/source/SoundTouch/SoundTouch.cpp
    soundtouch/source/SoundTouch/TDStretch.cpp
    soundtouch/source/SoundTouch/cpu_detect_x86.cpp
    #soundtouch/source/SoundTouch/mmx_optimized.cpp
    soundtouch/source/SoundTouch/sse_optimized.cpp
)

set(SOUNDTOUCH_FLOAT_SAMPLES 1)
set(SOUNDTOUCH_INTEGER_SAMPLES 0)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/soundtouch/include/soundtouch_config.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/soundtouch_config.h)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/soundtouch/include)
include_directories(${CMAKE_CURRENT_BINARY_DIR})
add_library(soundtouch STATIC ${SOUNDTOUCH_SRC})

if(UNIX)
    set_target_properties(soundtouch PROPERTIES COMPILE_FLAGS "-fPIC")
endif()
