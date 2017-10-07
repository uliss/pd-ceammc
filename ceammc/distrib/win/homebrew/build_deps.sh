#!/bin/bash

if [ $# -lt 1 ]
then
    echo "Usage: $(basename $0) package"
    exit 1
fi

PKG=$1
CWD=`pwd`

PREFIX=/opt/local/mingw32

export PATH=$PATH:/usr/local/bin
export PKG_CONFIG_PATH="${PREFIX}/lib/pkgconfig"
export TCLSH="/usr/bin/tclsh8.5"

ASIO_ARCHIVE_NAME="asiosdk2.3.zip"
ASIO_URL="http://www.steinberg.net/sdk_downloads/${ASIO_ARCHIVE_NAME}"

HOST=i686-w64-mingw32
CC=i686-w64-mingw32-gcc
CPP=i686-w64-mingw32-cpp
CXX=i686-w64-mingw32-c++
AR=i686-w64-mingw-ar


function banner() {
    echo "*****************************************************"
    tput setaf 2
    echo $1
    tput sgr0
    echo "*****************************************************"
}

function install_libmodplug() {
    pkg="libmodplug"
    cd "${CWD}"

    banner "${pkg}"
    rm -rf libmodplug*

    banner "Downloading ${pkg}"
    wget https://codeload.github.com/Konstanty/libmodplug/zip/master -O libmodplug.zip
    unzip -o libmodplug.zip
    cd libmodplug-*

    banner "Configure ${pkg}"
    autoreconf -fi
    ./configure --host=${HOST} \
    	--enable-static=yes \
    	--enable-shared=yes \
    	--prefix=${PREFIX}

    banner "Build ${pkg}"
    make

    banner "Installing ${pkg}"
    make install
}

function install_sndfile() {
    pkg="sndfile"
    cd "${CWD}"

    banner "${pkg}"
    rm -rf libsndfile*

    banner "Downloading ${pkg}"
    brew unpack libsndfile
    cd libsndfile*

    banner "Configure ${pkg}"
    CFLAGS="-I${PREFIX}/include" ./configure --host=${HOST} \
    	--enable-static=yes \
    	--enable-shared=yes \
    	--prefix=${PREFIX}

    banner "Build ${pkg}"
    make

    banner "Installing ${pkg}"
    make install
}

function install_ogg() {
    pkg="OGG"
    cd "${CWD}"

    banner "${pkg}"
    rm -rf libogg*

    banner "Downloading ${pkg}"
    brew unpack libogg
    cd libogg*

    banner "Configure ${pkg}"
    ./configure --host=${HOST} \
    	--enable-static=yes \
    	--enable-shared=yes \
    	--prefix=${PREFIX}

    banner "Build ${pkg}"
    make

    banner "Installing ${pkg}"
    make install
}

function install_portaudio() {
    pkg="Portaudio"
    cd "${CWD}"

    banner "${pkg}"
    rm -rf portaudio*

    banner "Downloading ${pkg}"

    if [ ! -f "asiosdk2.3.zip" ]
    then
        curl -O http://www.steinberg.net/sdk_downloads/asiosdk2.3.zip
    fi

    brew unpack portaudio
    unzip -o "asiosdk2.3.zip"

    mkdir -p ${PREFIX}/lib/asiosdk2
    cp -R ASIOSDK2.3 ${PREFIX}/lib/asiosdk2

    cd portaudio*
    perl -pi -e "s|-luuid||g" configure.in
    autoreconf -fi

    #make clean
    export CFLAGS="$CFLAGS -I./src/hostapi/wasapi/mingw-include"
    ./configure --host=${HOST} \
        --prefix=${PREFIX} \
        --with-winapi=asio,directx,wdmks,wmme \
        --enable-static=no \
    	--enable-shared=yes \
        --with-dxdir=$(pwd)/w32api-directx-standalone-master/include \
        --with-asiodir=${PREFIX}/lib/asiosdk2/ASIOSDK2.3

    make
    make install
}

function install_vorbis() {
    pkg="vorbis"
    cd "${CWD}"

    banner "${pkg}"
    rm -rf libvorbis*

    banner "Downloading ${pkg}"
    brew unpack libvorbis
    cd libvorbis*

    banner "Configure ${pkg}"
    ./configure --host=${HOST} \
    	--enable-static=yes \
    	--enable-shared=yes \
    	--prefix=${PREFIX}

    banner "Build ${pkg}"
    make

    banner "Installing ${pkg}"
    make install
}

function install_flac() {
    pkg="FLAC"
    cd "${CWD}"

    banner "${pkg}"
    rm -rf flac*

    banner "Downloading ${pkg}"
    brew unpack flac
    cd flac*

    banner "Configure ${pkg}"
    ./configure --host=${HOST} \
    	--enable-static=yes \
    	--enable-shared=yes \
        --disable-cpplibs \
    	--prefix=${PREFIX}

    banner "Build ${pkg}"
    make

    banner "Installing ${pkg}"
    make install
}

function install_fftw3() {
    pkg="FFTW3"
    cd "${CWD}"

    banner "${pkg}"
    rm -rf fftw*

    banner "Downloading ${pkg}"
    brew unpack fftw
    cd fftw-*

    banner "Configure ${pkg} float"
    ./configure --host=${HOST}\
        --enable-single \
        --prefix=${PREFIX} \
        --disable-fortran \
        --enable-sse \
        --enable-sse2 \
        --enable-avx \
        --enable-avx2 \
        --with-our-malloc \
        --enable-shared=yes \
        --enable-static=no

    banner "Build ${pkg} float"
    make

    banner "Install ${pkg} float"
    make install
}

function install_tcl() {
    pkg="TCL"
    cd "${CWD}"

    banner "${pkg}"
    rm -rf tcl-release

    banner "Downloading ${pkg}"
    if [ ! -f "tcl-release.zip" ]
    then
        wget "https://github.com/tcltk/tcl/archive/release.zip" -O "tcl-release.zip"
    fi

    unzip "tcl-release.zip"
    cd tcl-release/win

    banner "Configure ${pkg}"
    ./configure --host=${HOST}\
        --enable-shared \
        --enable-threads \
        --prefix=${PREFIX}

    banner "Build ${pkg}"
    make

    banner "Installing ${pkg}"
    make install
}

function install_tcllib() {
    pkg="tcllib"
    cd "${CWD}"

    banner "${pkg}"
    rm -rf tcllib-*

    banner "Downloading ${pkg}"
    if [ ! -f "tcllib.zip" ]
    then
        wget "https://github.com/tcltk/tcllib/archive/tcllib_1_18.zip" -O "tcllib.zip"
    fi

    unzip "tcllib.zip"
    cd tcllib-*

    banner "Configure ${pkg}"
    ./configure  --with-tclsh="/usr/bin/tclsh" \
        --prefix=${PREFIX}

    banner "Build ${pkg}"
    make

    banner "Installing ${pkg}"
    make install
}

function install_tklib() {
    pkg="tklib"
    cd "${CWD}"

    banner "${pkg}"
    rm -rf tklib-*

    banner "Downloading ${pkg}"
    if [ ! -f "tklib.zip" ]
    then
        wget "https://codeload.github.com/tcltk/tklib/zip/master" -O "tklib.zip"
    fi

    unzip "tklib.zip"
    LIBNAME=`basename tklib-*`
    cd tklib-*

    banner "Configure ${pkg}"
    ./configure --prefix=${PREFIX}

    banner "Build ${pkg}"
    FROM="/opt/local/mingw32/bin/tclsh86.exe"
    perl -pi -e "s|${FROM}|${TCLSH}|g" Makefile

    banner "Installing ${pkg}"
    make install
}


function install_tk() {
    pkg="TK"
    cd "${CWD}"

    banner "${pkg}"
    rm -rf tk-release

    banner "Downloading ${pkg}"
    if [ ! -f "tk-release.zip" ]
    then
        wget "https://github.com/tcltk/tk/archive/release.zip" -O tk-release.zip
    fi

    unzip "tk-release.zip"
    cd tk-release/win

    banner "Configure ${pkg}"
    ./configure --host=${HOST}\
        --enable-shared \
        --enable-threads \
        --prefix=${PREFIX}

    banner "Build ${pkg}"
    make

    banner "Installing ${pkg}"
    make install
}

function install_runtime() {
    pkg="MSVC runtime"
    cd "${CWD}"

    banner "${pkg}"
    mkdir -p msvcrt
    cd msvcrt

    if [ ! -f "vcredist_x86.exe" ]
    then
        wget https://download.microsoft.com/download/5/B/C/5BC5DBB3-652D-4DCE-B14A-475AB85EEF6E/vcredist_x86.exe
    fi

    cabextract -d vcredist "vcredist_x86.exe"
    cabextract "vcredist/vc_red.cab"
    cp "F_CENTRAL_msvcr100_x86" ${PREFIX}/bin/msvcr100.dll
}

function install_fonts() {
    pkg="DejaVu fonts"
    cd "${CWD}"

    banner "${pkg}"
    mkdir -p fonts
    cd fonts

    if [ ! -f "dejavu-fonts-ttf-2.37.zip" ]
    then
        wget https://github.com/dejavu-fonts/dejavu-fonts/releases/download/version_2_37/dejavu-fonts-ttf-2.37.zip
    fi

    unzip -o "dejavu-fonts-ttf-2.37.zip"
    cd dejavu-fonts-ttf*

    mkdir -p "${PREFIX}/pd/fonts"
    cp ttf/DejaVuSansMono.ttf "${PREFIX}/pd/fonts"
    cp ttf/DejaVuSans.ttf "${PREFIX}/pd/fonts"
    cp LICENSE "${PREFIX}/pd/fonts"
}

case ${PKG} in
    all)
        install_libmodplug
        install_fftw3
        install_tcl
        install_tcllib
        install_tk
        install_tklib
        install_ogg
        install_vorbis
        install_flac
        install_sndfile
        install_portaudio
        install_fonts
        instal_runtime
        ;;
    modplug)
        install_libmodplug
        ;;
    fftw3)
        install_fftw3
        ;;
    tcl)
        install_tcl
        ;;
    tcllib)
        install_tcllib
        ;;
    tk)
        install_tk
        ;;
    tklib)
        install_tklib
        ;;
    ogg)
        install_ogg
        ;;
    vorbis)
        install_vorbis
        ;;
    flac)
        install_flac
        ;;
    sndfile)
        install_sndfile
        ;;
    portaudio)
        install_portaudio
        ;;
    runtime)
        install_runtime
        ;;
    fonts)
        install_fonts
        ;;
    *)
        echo "Choose from following: modplug, fftw3, tcl, tcllib, tk, tklib, ogg, \
              vorbis, flac, portaudio or all"
        exit 1
        ;;
esac
