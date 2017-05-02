#!/bin/sh

SCRIPT_DIR="$(dirname $0)"
DIR=`pwd`
export BUILD_DIR="`pwd`/build/deps"
rm -rf "$BUILD_DIR"
mkdir -p "$BUILD_DIR/src"

cd "$BUILD_DIR/src"

function build {
    echo "Building $1 ..." 
    echo ""
    $SHELL "${DIR}/${SCRIPT_DIR}/build_$1.sh"
}

#build fftw
build ogg
#build opus
#build speex
#build flac
build vorbis
#build sndfile
build portaudio

