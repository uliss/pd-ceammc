#!/bin/bash

cmake -G Ninja \
	-DCMAKE_CXX_FLAGS=-ffast-math\
	-DCMAKE_C_FLAGS=-ffast-math \
	-DCMAKE_BUILD_TYPE=Release \
	-DWITH_FLUIDSYNTH=ON \
	-DWITH_DOUBLE_PRECISION=ON \
	-DARCH=x86_64 \
	-DLEAPMOTION_ROOT=~/work/src/LeapSDK \
	-DENABLE_TESTS=ON \
	-DWITH_BENCHMARK=OFF\
	-DWITH_FAUST=ON\
	../../../../..

