#!/bin/sh

CONF_FLAGS=""

MACOSX_MINOR=$(sw_vers | grep ProductVersion | cut -f2 | cut -d. -f2)
if [ ${MACOSX_MINOR} -le 7 ]
then
	echo "MacOSX 10.$MACOSX_MINOR found..."
	echo ""
	CONF_FLAGS="${CONF_FLAGS} --disable-asm-optimizations --disable-dependency-tracking"
	export CC=gcc
fi

export CFLAGS='-arch x86_64 -arch i386 -O2'
PREFIX="$BUILD_DIR"

if [ -z "$PREFIX" ]
then
    PREFIX=`pwd`/build/deps
fi

export PKG_CONFIG_PATH="${PREFIX}/lib/pkgconfig"

rm -rf flac-*
brew unpack flac
cd flac-*
./configure --enable-shared=yes \
    --disable-asm-optimizations \
    --enable-static=no \
    --disable-cpplibs \
    --prefix=${PREFIX} \
    ${CONF_FLAGS}

make
make install
