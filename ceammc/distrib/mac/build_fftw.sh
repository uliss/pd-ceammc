#!/bin/sh

export CFLAGS='-arch x86_64 -arch i386 -O2'
PREFIX="$BUILD_DIR"
if [ -z "$PREFIX" ]
then
    PREFIX=`pwd`/build/deps
fi

CONF_FLAGS="--disable-fortran --disable-dependency-tracking"
MV=$(sw_vers -productVersion | cut -d. -f2)
if [ $MV -ge 9 ]
then
    echo "AVX support ON"
    CONF_FLAGS="$CONF_FLAGS --enable-avx"
fi

export PKG_CONFIG_PATH="${PREFIX}/lib/pkgconfig"

rm -rf fftw-*
brew unpack fftw
cd fftw-*
./configure --enable-shared=yes \
    --enable-static=no \
    --enable-single \
    ${CONF_FLAGS} \
    --prefix=${PREFIX}

make
make install



