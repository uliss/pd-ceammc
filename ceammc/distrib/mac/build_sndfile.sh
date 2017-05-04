#!/bin/sh

export CFLAGS='-arch x86_64 -arch i386 -O2'
PREFIX="$BUILD_DIR"
if [ -z "$PREFIX" ]
then
    PREFIX=`pwd`/build/deps
fi

export PKG_CONFIG_PATH=${PREFIX}/lib/pkgconfig
export CFLAGS="${CFLAGS} -I${PREFIX}/include"


MV=$(sw_vers -productVersion | cut -d. -f2)
if [ $MV -eq 7 ]
then
    echo "Mac 10.7 found..."
    export CC=cc
fi

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
