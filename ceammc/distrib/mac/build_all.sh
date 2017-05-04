#!/bin/sh

SCRIPT_DIR="$(dirname $0)"
DIR=`pwd`
export BUILD_DIR="`pwd`/build/deps"
export PKG_CONFIG_PATH="${BUILD_DIR}/lib/pkgconfig"
export PKG_CONFIG_LIBDIR=${PKG_CONFIG_PATH}
rm -rf "$BUILD_DIR"
mkdir -p "$BUILD_DIR/src"

cd "$BUILD_DIR/src"

function build {
    echo "Building $1 ..." 
    echo ""
    $SHELL "${DIR}/${SCRIPT_DIR}/build_$1.sh"
}

build fftw
build ogg
#build opus
#build speex
build flac
build vorbis
build sndfile
build portaudio

export BUILD_DIR="${DIR}/build/pd"
build pd

mv ${DIR}/build/pd/*.dmg "${DIR}/build"
mv ${DIR}/build/pd/ceammclib-*tar.gz "${DIR}/build"
