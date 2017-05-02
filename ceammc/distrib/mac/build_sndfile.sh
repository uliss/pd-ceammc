#!/bin/sh

export CFLAGS='-arch x86_64 -arch i386 -O2'
PREFIX=`pwd`/../build
export PKG_CONFIG_PATH=${PREFIX}/lib/pkgconfig
#export PKG_CONFIG_LIBDIR=${PKG_CONFIG_PATH}

rm -rf libsndfile-*
brew unpack libsndfile
cd libsndfile-*
./configure --enable-shared=yes \
    --enable-stack-smash-protection \
    --disable-sqlite \
    --disable-octave \
    --enable-static=no \
    --prefix=${PREFIX}

make
make install
