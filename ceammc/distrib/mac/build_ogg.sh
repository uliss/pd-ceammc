#!/bin/sh

export CFLAGS='-arch x86_64 -arch i686 -O2'
PREFIX=`pwd`/../build
export PKG_CONFIG_PATH="${PREFIX}/lib/pkgconfig"

rm -rf libogg*
brew unpack libogg
cd libogg-*
./configure --enable-shared=yes \
    --enable-static=no \
    --prefix=${PREFIX}

make
make install

#install_name_tool -id "@loader_path/libogg.0.dylib" "${PREFIX}/lib/libogg.0.dylib"

