#!/bin/sh

export CFLAGS='-arch x86_64 -arch i386 -O2'
export LDFLAGS='-arch x86_64 -arch i386 -O2'
PREFIX="$BUILD_DIR"
if [ -z "$PREFIX" ]
then
    PREFIX=`pwd`/build/deps
fi

export PKG_CONFIG_PATH="${PREFIX}/lib/pkgconfig"

rm -rf libogg*
brew unpack libogg
cd libogg-*
./configure --enable-shared=yes \
    --enable-static=no \
    --disable-dependency-tracking \
    --prefix=${PREFIX}

make
make install
