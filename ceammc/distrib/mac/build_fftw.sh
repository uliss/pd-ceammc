#!/bin/sh

export CFLAGS='-arch x86_64 -arch i386 -O2'
PREFIX="$BUILD_DIR"
export PKG_CONFIG_PATH="${PREFIX}/lib/pkgconfig"

rm -rf fftw-*
brew unpack fftw
cd fftw-*
./configure --enable-shared=yes \
    --enable-static=no \
    --enable-single \
    --enable-avx \
    --disable-fortran \
    --prefix=${PREFIX}

make
make install



