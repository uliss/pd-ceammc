#!/bin/sh

export CFLAGS='-arch x86_64 -arch i386 -O2'
PREFIX="$BUILD_DIR"
if [ -z "$PREFIX" ]
then
    PREFIX=`pwd`/build/deps
fi

export PKG_CONFIG_PATH=${PREFIX}/lib/pkgconfig
export CFLAGS="${CFLAGS} -I${PREFIX}/include"
#export PKG_CONFIG_LIBDIR=${PKG_CONFIG_PATH}

rm -rf libsndfile-*
brew unpack libsndfile
cd libsndfile-*
./configure --enable-shared=yes \
    --enable-stack-smash-protection \
    --disable-dependency-tracking \
    --disable-sqlite \
    --disable-octave \
    --enable-static=no \
    --prefix=${PREFIX}

make
make install
