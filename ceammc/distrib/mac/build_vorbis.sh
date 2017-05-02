#!/bin/sh

export CFLAGS='-arch x86_64 -arch i386 -O2'
PREFIX=`pwd`/../build
export PKG_CONFIG_PATH=${PREFIX}/lib/pkgconfig
echo $PKG_CONFIG_PATH

rm -rf libvorbis*
brew unpack libvorbis
cd libvorbis-*
./configure --enable-shared=yes \
    --enable-static=no \
    --with-sysroot=${PREFIX} \
    --with-ogg=${PREFIX} \
    --prefix=${PREFIX}

make
make install

