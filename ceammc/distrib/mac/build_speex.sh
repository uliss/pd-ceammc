#!/bin/sh

export CFLAGS='-arch x86_64 -arch i686 -O2'
PREFIX=`pwd`/../build
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

LSPEEX="libspeex.1.dylib"
#install_name_tool -id "@loader_path/${LSPEEX}" "${PREFIX}/lib/${LSPEEX}"

