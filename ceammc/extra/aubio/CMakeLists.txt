set(AUBIO_SRC
    aubio/src/cvec.c
    aubio/src/fmat.c
    aubio/src/fvec.c
    aubio/src/lvec.c
    aubio/src/mathutils.c
    aubio/src/musicutils.c
    aubio/src/vecutils.c
#    aubio/src/io/audio_unit.c
    aubio/src/io/ioutils.c
    aubio/src/io/sink.c
#    aubio/src/io/sink_apple_audio.c
#    aubio/src/io/sink_sndfile.c
    aubio/src/io/sink_wavwrite.c
    aubio/src/io/source.c
#    aubio/src/io/source_apple_audio.c
#    aubio/src/io/source_avcodec.c
#    aubio/src/io/source_sndfile.c
    aubio/src/io/source_wavread.c
#    aubio/src/io/utils_apple_audio.c
    aubio/src/notes/notes.c
    aubio/src/onset/onset.c
    aubio/src/onset/peakpicker.c
    aubio/src/pitch/pitch.c
    aubio/src/pitch/pitchfcomb.c
    aubio/src/pitch/pitchmcomb.c
    aubio/src/pitch/pitchschmitt.c
    aubio/src/pitch/pitchspecacf.c
    aubio/src/pitch/pitchyin.c
    aubio/src/pitch/pitchyinfast.c
    aubio/src/pitch/pitchyinfft.c
    aubio/src/spectral/awhitening.c
    aubio/src/spectral/dct.c
#    aubio/src/spectral/dct_accelerate.c
    aubio/src/spectral/dct_fftw.c
#    aubio/src/spectral/dct_ipp.c
#    aubio/src/spectral/dct_ooura.c
    aubio/src/spectral/dct_plain.c
    aubio/src/spectral/fft.c
    aubio/src/spectral/filterbank.c
    aubio/src/spectral/filterbank_mel.c
    aubio/src/spectral/mfcc.c
    aubio/src/spectral/ooura_fft8g.c
    aubio/src/spectral/phasevoc.c
    aubio/src/spectral/specdesc.c
    aubio/src/spectral/statistics.c
    aubio/src/spectral/tss.c
    aubio/src/synth/sampler.c
    aubio/src/synth/wavetable.c
    aubio/src/tempo/beattracking.c
    aubio/src/tempo/tempo.c
    aubio/src/temporal/a_weighting.c
    aubio/src/temporal/biquad.c
    aubio/src/temporal/c_weighting.c
    aubio/src/temporal/filter.c
    aubio/src/temporal/resampler.c
    aubio/src/utils/hist.c
    aubio/src/utils/log.c
    aubio/src/utils/parameter.c
    aubio/src/utils/scale.c
#    aubio/src/utils/windll.c
    aubio/src/effects/pitchshift.h
    aubio/src/effects/pitchshift_dummy.c
    aubio/src/effects/pitchshift_rubberband.c
    aubio/src/effects/rubberband_utils.c
    aubio/src/effects/timestretch.h
    aubio/src/effects/timestretch_dummy.c
    aubio/src/effects/timestretch_rubberband.c
)

add_library(aubio STATIC ${AUBIO_SRC})
target_include_directories(aubio PRIVATE
    ${PROJECT_BINARY_DIR}
     aubio/src)
target_compile_definitions(aubio PRIVATE -DHAVE_CONFIG_H -DHAVE_WAVWRITE -DHAVE_WAVREAD)

check_include_file_cxx("complex" HAVE_COMPLEX)
if(HAVE_COMPLEX)
    target_compile_definitions(aubio PRIVATE -DHAVE_COMPLEX_H)
endif()

if(WITH_FFTW)
    target_compile_definitions(aubio PRIVATE -DHAVE_FFTW3 -DHAVE_FFTW3F)
    target_link_libraries(aubio PRIVATE ${FFTW_LIBRARIES})
endif()

if(UNIX)
    set_target_properties(aubio PROPERTIES COMPILE_FLAGS "-fPIC")
endif()
