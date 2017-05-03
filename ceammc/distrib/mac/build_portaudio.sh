#!/bin/sh

PREFIX="$BUILD_DIR"
if [ -z "$PREFIX" ]
then
    PREFIX=`pwd`/build/deps
fi

echo "Building PortAudio..."
echo ""
echo "    build root: ${PREFIX}"
echo ""

rm -rf portaudio-*
brew unpack portaudio
cd portaudio-*


export PKG_CONFIG_PATH="$PREFIX/lib/pkgconfig"
cmake -DCMAKE_OSX_ARCHITECTURES="x86_64;i386" \
    -DCMAKE_C_FLAGS=$CFLAGS \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX=${PREFIX}

make
make install

# fix
PA="${PREFIX}/lib/libportaudio.dylib"
install_name_tool -id $PA $PA
