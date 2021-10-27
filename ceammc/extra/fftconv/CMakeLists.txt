add_library(fftconv STATIC FFTConvolver/AudioFFT.cpp)

target_sources(fftconv
    PRIVATE FFTConvolver/AudioFFT.cpp
            FFTConvolver/FFTConvolver.cpp
            FFTConvolver/TwoStageFFTConvolver.cpp
            FFTConvolver/Utilities.cpp)

target_include_directories(fftconv INTERFACE FFTConvolver)

if(APPLE)
    target_compile_definitions(fftconv PRIVATE AUDIOFFT_APPLE_ACCELERATE)
    target_link_libraries(fftconv PUBLIC "-framework Accelerate")
elseif(FFTW_FOUND)
    target_compile_definitions(fftconv PRIVATE AUDIOFFT_FFTW3)
endif()
