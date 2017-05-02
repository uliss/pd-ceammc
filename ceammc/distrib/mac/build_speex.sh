#!/bin/sh

export CFLAGS='-arch x86_64 -arch i386 -O2'
PREFIX="$BUILD_DIR"
export PKG_CONFIG_PATH="${PREFIX}/lib/pkgconfig"

rm -rf speex-*
brew unpack speex
cd speex-*
./configure --enable-shared=yes \
    --enable-static=no \
    --enable-sse  \
    --disable-binaries \
    --prefix=${PREFIX}

make
make install

