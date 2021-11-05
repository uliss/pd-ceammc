#/bin/bash

cmake -G Ninja -DWITH_COVERAGE=ON \
    -DWITH_PORTAUDIO=OFF \
    -DWITH_DUMMY_AUDIO=ON \
    -DWITH_FLUIDSYNTH=OFF \
    -DWITH_TTS_FLITE=OFF \
    -DWITH_MODPLUG=OFF \
    -DWITH_EXT_FLEXT=OFF \
    -DWITH_EXT_FFTEASE=OFF \
    -DWITH_EXT_LYONPOTPOURRI=OFF \
    -DWITH_EXT_AUTOTUNE=OFF \
    -DWITH_FFTW=OFF \
    ../../../..
