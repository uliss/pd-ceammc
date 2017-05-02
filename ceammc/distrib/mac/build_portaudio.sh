#!/bin/sh

PREFIX=`pwd`/../build

rm -rf portaudio-*
brew unpack portaudio
cd portaudio-*
cmake -DCMAKE_OSX_ARCHITECTURES="x86_64;i386" \
    -DCMAKE_C_FLAGS=$CFLAGS \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX=${PREFIX}

make
make install
