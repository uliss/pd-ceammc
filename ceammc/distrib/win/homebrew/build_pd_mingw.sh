#!/bin/bash

if [ $# -ne 1 ]
then
    echo "Usage $(basename $0) PATH_TO_PD..."
    exit 1
fi

PD_DIR=$1
CWD=`pwd`

PREFIX="/opt/local/mingw32"
CURRENT_DATE=$(LANG=C date -u '+%d %h %Y %Z %H:%M:%S')
GIT_BRANCH=$(git --git-dir $PD_DIR/.git symbolic-ref --short HEAD)
GIT_COMMIT=$(git --git-dir $PD_DIR/.git describe --tags)

export PKG_CONFIG_PATH="${PREFIX}/lib/pkgconfig"
ls ${PKG_CONFIG_PATH}

HOST=i686-w64-mingw32
CC=${HOST}-gcc
CPP=${HOST}-cpp
CXX=${HOST}-c++
AR=${HOST}-ar
TC="$CWD/Toolchain-mingw32.cmake"

if [ ! -f $TC ]; then
	echo "Toolchain file not found: $TC"
	echo "Copy file from ${PD_DIR}/ceammc/distrib/win"
	echo " and edit for your system"
	exit 1
fi

mkdir -p pd
cd pd
#rm -rf *

export ProgramW6432="${HOME}/.wine/drive_c/Program Files/"
export PROGRAMFILES="${HOME}/.wine/drive_c/Program Files/"
export DNSSD_ROOT="${HOME}/.wine/drive_c/Program\ Files/Bonjour SDK"

cmake -DCMAKE_TOOLCHAIN_FILE=$TC \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_PREFIX_PATH=${PREFIX} \
    -DBOOST_ROOT=/usr/local/include \
    -DWITH_FFTW=ON \
    -DFFTW_ROOT=/opt/local/mingw32 \
    -DWITH_DUMMY_AUDIO=OFF \
    -DWITH_DUMMY_MIDI=OFF \
    -DWITH_PORTAUDIO=ON \
    -DWITH_MMIO=OFF \
    -DCMAKE_INSTALL_PREFIX=${PREFIX}/pd \
    ${PD_DIR}/..

make -j3 install

#echo "Copy about file..."
#cat ceammc/ext/doc/about.pd | sed "s/%GIT_BRANCH%/$GIT_BRANCH/g" | \
#   sed "s/%GIT_COMMIT%/$GIT_COMMIT/g" | \
#   sed "s/%BUILD_DATE%/$CURRENT_DATE/g" > tmp.about.pd
#cp tmp.about.pd ceammc/ext/doc/about.pd
