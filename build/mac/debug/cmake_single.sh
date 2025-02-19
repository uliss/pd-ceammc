#!/bin/bash

# M1 apple check (Faust not build at this moment)
BUILD_WITH_FAUST=ON
if [[ $(uname -m) == 'arm64' ]]; then
  BUILD_WITH_FAUST=ON
  export LLVM_DIR=/opt/local/libexec/llvm-11/lib/cmake/llvm
fi


cmake -G Ninja \
	-DCMAKE_CXX_FLAGS=-ffast-math\
	-DCMAKE_C_FLAGS=-ffast-math \
	-DCMAKE_BUILD_TYPE=Debug \
	-DCMAKE_INSTALL_PREFIX=`pwd`/dist/pd_ceammc \
	-DWITH_FLUIDSYNTH=ON \
	-DARCH=x86_64 \
	-DLEAPMOTION_ROOT=~/work/src/LeapSDK \
	-DENABLE_TESTS=ON \
	-DWITH_EXT_FLEXT=OFF \
	-DWITH_FAUST=$BUILD_WITH_FAUST \
	-DWITH_BENCHMARK=ON\
	../../../..

