#!/bin/sh

export CFLAGS='-arch x86_64 -arch i386 -O2'
PREFIX="$BUILD_DIR"
export PKG_CONFIG_PATH="${PREFIX}/lib/pkgconfig"

rm -rf libogg*
brew unpack libogg
cd libogg-*
./configure --enable-shared=yes \
    --enable-static=no \
    --prefix=${PREFIX}

make
make install
