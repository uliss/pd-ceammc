#!/bin/sh

export CFLAGS='-arch x86_64 -arch i686 -O2'
#export LDFLAGS='-rpath @rpath/libvorbis.0.dylib'
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

LIBOGG="libogg.0.dylib"
LIBVORBIS="libvorbis.0.dylib"
LIB="${PREFIX}/lib/${LIBVORBIS}"
LIBVORBISENC="libvorbisenc.2.dylib"
LIBENC="${PREFIX}/lib/${LIBVORBISENC}"

#install_name_tool -id "@loader_path/${LIBVORBIS}" "${LIB}"
#install_name_tool -id "@loader_path/${LIBVORBISENC}" "${LIBENC}"
#install_name_tool -change "${LIB}" "@loader_path/${LIBVORBIS}" "${LIBENC}"

