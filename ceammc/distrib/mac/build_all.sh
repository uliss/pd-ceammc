#!/bin/sh

SCRIPT_DIR="$(dirname $0)"
DIR=`pwd`
export BUILD_DIR="`pwd`/build/deps"
export PKG_CONFIG_PATH="${BUILD_DIR}/lib/pkgconfig"
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

cd "${BUILD_DIR}/.."

echo "Building PureData ..."
rm -rf pd
mkdir pd
cd pd

cmake -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_C_FLAGS='-O2' \
    -DCMAKE_CXX_FLAGS='-O2' \
    -DARCH='x86_64;i386' \
    -DFFTW_ROOT="${BUILD_DIR}" \
    "${DIR}"

cmake "${DIR}"
make
make test
make app
make dmg
make ceammc_lib
make ceammc_lib_compat



