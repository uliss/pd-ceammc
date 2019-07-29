option(WITH_COVERAGE  "Build with coverage and profiling flags" OFF)
option(WITH_FLUIDSYNTH "Build with FluidLite support" ON)
option(WITH_EXPERIMENTAL "Build with experimental objects" OFF)
option(WITH_BENCHMARK "Build with benchmarks" ON)
option(WITH_MODPLUG "Build with libmodplug support" ON)
option(WITH_FFTW "Use fftw3 library (http://www.fftw.org/)" ON)

if(UNIX)
    option(WITH_ASAN "Build with address sanitizer" OFF)
endif()

option(ENABLE_TESTS "Enable tests" ON)
