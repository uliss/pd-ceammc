#!/bin/sh

export CFLAGS='-arch x86_64 -arch i686 -O2'
PREFIX=`pwd`/../build
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

#LFLAC="libFLAC.8.dylib"
#install_name_tool -id "@loader_path/${LFLAC}" "${PREFIX}/lib/${LFLAC}"

