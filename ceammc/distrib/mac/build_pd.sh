#!/bin/bash

PREFIX=/opt/local/universal
READLINK=$(which greadlink)

error() {
    tput setaf 1
    printf "Error: "
    tput sgr0
    echo $*
}

if [ -z ${READLINK} ]
then
    error "GNU readlink is not found..."
    exit 1
fi

PD_SRC_DIR="$(dirname $0)/../../.."
PD_SRC_DIR=$(${READLINK} -f ${PD_SRC_DIR})

if [ ! -f "${PD_SRC_DIR}/CMakeLists.txt" ]
then
    error "not a Pd source directory: \"${PD_SRC_DIR}\""
    exit 1
fi

if [ ! "$1" = "rebuild" ]
then
    echo "Remove old build directory..."
    rm -rf pd
    mkdir -p pd
fi

cd pd

echo "Building PureData..."
echo ""
echo "    build root: ${PREFIX}"
echo "    Pd source:  ${PD_SRC_DIR}"
echo ""

export LIBRARY_PATH="${PREFIX}/lib"
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

export PKG_CONFIG_PATH="${PREFIX}/lib/pkgconfig"
cmake -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_C_FLAGS='' \
    -DCMAKE_CXX_FLAGS='-funroll-loops -fomit-frame-pointer' \
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
