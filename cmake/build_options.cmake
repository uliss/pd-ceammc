option(WITH_COVERAGE  "Build with coverage and profiling flags" OFF)
option(WITH_EXPERIMENTAL "Build with experimental objects" OFF)
option(WITH_BENCHMARK "Build with benchmarks" ON)
option(WITH_CLANG_TIDY "Build with clang-tidy checks" OFF)

option(WITH_TTS_FLITE "Build with Flite TTS support" ON)
option(WITH_FLUIDSYNTH "Build with FluidSynth support" ON)
option(WITH_FFTW "Use fftw3 library (http://www.fftw.org/)" ON)
option(WITH_MODPLUG "Build with libmodplug support" ON)
option(WITH_EXT_LYONPOTPOURRI "Build lyonpotpourri externals" ON)
option(WITH_EXT_FFTEASE "Build FFTease externals" ON)
option(WITH_EXT_AUTOTUNE "Build autotune external" ON)
option(WITH_EXT_FLEXT "Build flext externals" ON)


if(UNIX)
    option(WITH_ASAN "Build with address sanitizer" OFF)
    option(WITH_PROFILE "Build with gprof support" OFF)
endif()

option(ENABLE_TESTS "Enable tests" ON)

