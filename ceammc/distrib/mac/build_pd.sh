#!/bin/bash

PREFIX=/opt/local/universal
PD_SRC_DIR=$1/..

#rm -rf pd
mkdir -p pd
cd pd

echo "Building PureData..."
echo ""
echo "    build root: ${PREFIX}"
echo ""

CXX_COMPILER=""
C_COMPILER=""

MACOSX_MINOR=$(sw_vers | grep ProductVersion | cut -f2 | cut -d. -f2)
ARCH='x86_64;i386'

if [ ${MACOSX_MINOR} -le 7 ]
then
        echo "MacOSX 10.$MACOSX_MINOR found..."
        echo ""
        CONF_FLAGS="${CONF_FLAGS} --disable-asm-optimizations --disable-dependency-tracking"
        ver=$(ls -1 /usr/local/bin/gcc-*  | cut -f2 -d- | sort | grep -Eo [0-9.]+ | sort | tail -1)
        C_COMPILER="-DCMAKE_C_COMPILER=/usr/local/bin/gcc-$ver"
        export CC="/usr/local/bin/gcc-$ver"
        CXX_COMPILER="-DCMAKE_CXX_COMPILER=/usr/local/bin/g++-$ver"
        ARCH='i386'
fi


DEPS_ROOT="${PREFIX}/../deps"

export PKG_CONFIG_PATH="${PREFIX}/lib/pkgconfig"
cmake -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_C_FLAGS='-O2' \
    -DCMAKE_CXX_FLAGS='-O2' \
    -DARCH=$ARCH \
    -DLEAPMOTION_ROOT="${HOME}/work/misc/LeapMotionSDK/LeapSDK" \
    ${C_COMPILER} \
    ${CXX_COMPILER} \
    -DLIBSNDFILE_ROOT="${PREFIX}" \
    -DPORTAUDIO_ROOT="${PREFIX}" \
    ${PD_SRC_DIR}

cmake ${PD_SRC_DIR}

function mytest {
    "$@"
    local status=$?
    if [ $status -ne 0 ]; then
        tput setaf 1
        echo "Error:"
        tput sgr0
        echo "      with $1" >&2
        tput sgr0
        exit 1
    fi
    return $status
}

rm -rf dist

mytest make -j3
mytest make test
mytest make app
mytest make dmg
mytest make ceammc_lib
mytest make ceammc_lib_extended
