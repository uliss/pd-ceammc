#!/bin/sh

export CFLAGS='-arch x86_64 -arch i686 -O2'
PREFIX=`pwd`/../build
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

#FFTW="libfftw3f.3.dylib"
#install_name_tool -id "@loader_path/${FFTW}" "${PREFIX}/lib/${FFTW}"


