#!/bin/bash

DIR="../../../../.."
BUILD="Release"
SUBDIR=$(echo $BUILD | tr '[:upper:]' '[:lower:]')
ARCH=$(echo $MSYSTEM | cut -c6-)
TYPE="double"
ROOT="/opt/local/pd/mingw${ARCH}/${SUBDIR}/${TYPE}"

cmake -G Ninja \
	-DCMAKE_BUILD_TYPE=$BUILD \
	-DCMAKE_INSTALL_PREFIX=${ROOT} \
	-DWITH_FFTW=ON \
	-DENABLE_TESTS=ON \
	-DWITH_FAUST=ON \
	-DWITH_EXT_LYONPOTPOURRI=OFF \
	-DWITH_EXT_FFTEASE=OFF \
	-DWITH_DOUBLE_PRECISION=ON \
	-DWITH_EXT_FLEXT=OFF \
	-DWITH_TTS_RHVOICE=ON \
	$DIR
