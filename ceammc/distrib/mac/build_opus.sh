#!/bin/sh

export CFLAGS='-arch x86_64 -arch i386 -O2'
PREFIX="$BUILD_DIR"
export PKG_CONFIG_PATH="${PREFIX}/lib/pkgconfig"

rm -rf opus-*
brew unpack opus
cd opus-*
./configure --enable-shared=yes \
    --enable-static=no \
    --disable-extra-programs \
    --prefix=${PREFIX}

make
make install
