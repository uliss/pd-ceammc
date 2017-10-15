#!/bin/bash

PWD=$(pwd)

ASIO_ARCHIVE_NAME="asiosdk2.3.zip"
ASIO_URL="http://www.steinberg.net/sdk_downloads/${ASIO_ARCHIVE_NAME}"
ASIO_SAVE_DIR="/usr/local"
ASIO_ARCHIVE="${ASIO_SAVE_DIR}/${ASIO_ARCHIVE_NAME}"
PA_ARHIVE_NAME="pa_stable_v19_20140130.tgz"
PA_URL="http://www.portaudio.com/archives/${PA_ARHIVE_NAME}"
FFTW3_ARCHIVE_NAME="fftw-3.3.5.tar.gz"
FFTW3_URL="http://www.fftw.org/${FFTW3_ARCHIVE_NAME}"
FFTW3_DIR=${FFTW3%.tar.gz}

echo "*****************************************************"
echo "Downloading ASIO SDK..."
echo "*****************************************************"

wget -P "$ASIO_SAVE_DIR" "$ASIO_URL"
cd "$ASIO_SAVE_DIR"
unzip -o "$ASIO_ARCHIVE_NAME"
mv "$ASIO_ARCHIVE_NAME" "asiosdk2"
echo "ASIO unpacked to $ASIO_SAVE_DIR"

mkdir -p build
cd build
echo "*****************************************************"
echo "Downloading PortAudio library to $(pwd)"
echo "*****************************************************"
wget "$PA_URL" 
tar zxvf $PA_ARHIVE_NAME
echo "done."

cd portaudio

echo ""
echo "*****************************************************"
echo "Configure PortAudio..."
echo "*****************************************************"
./configure --prefix=/usr --with-host_os=mingw --with-winapi=wmme,asio

echo ""
echo "*****************************************************"
echo "Build PortAudio..."
echo "*****************************************************"
make
echo "Install PortAudio..."
make install

cd ..
echo "*****************************************************"
echo "Downloading FFTW3 library to $(pwd)"
echo "*****************************************************"
wget "${FFTW3_URL}"
tar zvxf "${FFTW3_ARCHIVE_NAME}"
cd "${FFTW3_DIR}"

echo ""
echo "*****************************************************"
echo "Configure FFTW3 float..."
echo "*****************************************************"
./configure  --enable-single

echo ""
echo "*****************************************************"
echo "Build FFTW3 float ..."
echo "*****************************************************"
make
echo "Install FFTW3 float..."
make install

echo ""
echo "*****************************************************"
echo "Configure FFTW3 double..."
echo "*****************************************************"
./configure  --enable-double

echo ""
echo "*****************************************************"
echo "Build FFTW3 double ..."
echo "*****************************************************"
make
echo "Install FFTW3 double..."
make install
