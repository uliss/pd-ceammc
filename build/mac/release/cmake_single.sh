#!/bin/bash

export LLVM_DIR=/opt/local/libexec/llvm-10/lib/cmake/llvm
cmake -G Ninja \
	-DCMAKE_CXX_FLAGS=-ffast-math\
	-DCMAKE_C_FLAGS=-ffast-math \
	-DCMAKE_BUILD_TYPE=Release \
	-DCMAKE_INSTALL_PREFIX=`pwd`/dist/pd_ceammc \
	-DWITH_FLUIDSYNTH=ON \
	-DARCH=x86_64 \
	-DLEAPMOTION_ROOT=~/work/src/LeapSDK \
	-DENABLE_TESTS=ON \
	-DWITH_BENCHMARK=ON\
	../../../..

