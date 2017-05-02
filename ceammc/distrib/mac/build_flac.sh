#!/bin/sh

export CFLAGS='-arch x86_64 -arch i386 -O2'
PREFIX="$BUILD_DIR"
export PKG_CONFIG_PATH="${PREFIX}/lib/pkgconfig"

rm -rf flac-*
brew unpack flac
cd flac-*
./configure --enable-shared=yes \
    --disable-asm-optimizations \
    --enable-static=no \
    --disable-cpplibs \
    --prefix=${PREFIX}

make
make install
