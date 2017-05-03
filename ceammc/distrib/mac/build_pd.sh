#!/bin/bash

PREFIX="$BUILD_DIR"
if [ -z "$PREFIX" ]
then
    PREFIX=`pwd`/build/pd
fi

echo "Building PureData..."
echo ""
echo "    build root: ${PREFIX}"
echo ""

CXX_COMPILER=""
C_COMPILER=""

MACOSX_MINOR=$(sw_vers | grep ProductVersion | cut -f2 | cut -d. -f2)
if [ ${MACOSX_MINOR} -le 7 ]
then
        echo "MacOSX 10.$MACOSX_MINOR found..."
        echo ""
        CONF_FLAGS="${CONF_FLAGS} --disable-asm-optimizations --disable-dependency-tracking"
        gcc=$(ls -1 /usr/local/bin/gcc-*  | cut -f2 -d- | sort | grep '\d' | sort | tail -1)
        gxx=$(ls -1 /usr/local/bin/g++-*  | cut -f2 -d- | sort | grep '\d' | sort | tail -1)
        export CC=$gcc
        C_COMPILER="-DCMAKE_C_COMPILER=$gcc"
        CXX_COMPILER="-DCMAKE_CXX_COMPILER=$gxx"
fi

mkdir -p ${PREFIX}
cd ${PREFIX}
PD_SRC_DIR="${PREFIX}/../.."

export PKG_CONFIG_LIBDIR="${PREFIX}/lib/pkgconfig"
cmake -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_C_FLAGS='-O2' \
    -DCMAKE_CXX_FLAGS='-O2' \
    -DARCH='x86_64;i386' \
    -DFFTW_ROOT="${PREFIX}/../deps" \
    ${C_COMPILER} \
    ${CXX_COMPILER} \
    ${PD_SRC_DIR}

cmake ${PD_SRC_DIR}

make
make test
make app
make dmg
make ceammc_lib
make ceammc_lib_compat

