add_library(fftconv STATIC FFTConvolver/AudioFFT.cpp)

target_sources(fftconv
    PRIVATE FFTConvolver/AudioFFT.cpp
            FFTConvolver/FFTConvolver.cpp
            FFTConvolver/TwoStageFFTConvolver.cpp
            FFTConvolver/Utilities.cpp)

if(APPLE)
    target_compile_definitions(fftconv PRIVATE AUDIOFFT_APPLE_ACCELERATE)
elseif(FFTW_FOUND)
    target_compile_definitions(fftconv PRIVATE AUDIOFFT_FFTW3)
endif()
