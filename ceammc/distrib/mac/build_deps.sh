#!/bin/bash

if [ $# -lt 1 ]
then
    echo "Usage: $(basename $0) package"
    exit 1
fi

PKG=$1
CWD=`pwd`
OSX_MINOR_VER=`sw_vers -productVersion | cut -d. -f2`
OSX_OLD=0

PREFIX=/opt/local/universal
export PKG_CONFIG_PATH="${PREFIX}/lib/pkgconfig"

if [ $OSX_MINOR_VER -lt 8 ]; then
    echo "Old OSX version: 10.${OSX_MINOR_VER}"
    export CFLAGS='-arch i386 -O2'
    OSX_OLD=1
else
    echo "Mac OSX version: 10.${OSX_MINOR_VER}"
    export CFLAGS='-arch x86_64 -arch i386 -O2'
fi

echo "CFLAGS: ${CFLAGS}"

function banner() {
    echo "*****************************************************"
    tput setaf 2
    echo $1
    tput sgr0
    echo "*****************************************************"
}

function err() {
    echo "*****************************************************"
    tput setaf 1
    echo Error: $1
    tput sgr0
    echo "*****************************************************"
}

function install_libmodplug() {
    pkg="libmodplug"
    cd "${CWD}"

    banner "${pkg}"
    rm -rf libmodplug*

    banner "Downloading ${pkg}"
    curl https://codeload.github.com/Konstanty/libmodplug/zip/master -o libmodplug.zip
    unzip -o libmodplug.zip
    cd libmodplug-*

    banner "Configure ${pkg}"
    autoreconf -fi
    export CXXFLAGS="$CFLAGS"
    ./configure --enable-static=no \
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

    if [ $OSX_OLD -eq 1 ]; then
        export CC=/usr/local/bin/gcc-5
        echo "using sepcified gcc on old MacOSX version: ${CC}"
    fi

    CFLAGS="${CFLAGS} -I${PREFIX}/include" ./configure --enable-static=yes \
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
    ./configure --enable-static=yes \
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
    brew unpack portaudio

    cd portaudio*
    cmake -DCMAKE_C_COMPILER=clang \
	-DCMAKE_BUILD_TYPE=Release \
	-DCMAKE_INSTALL_PREFIX=${PREFIX} \
        -DCMAKE_SKIP_RPATH=TRUE \
        -DCMAKE_SKIP_BUILD_RPATH=FALSE \
        .

    make
    make install
    LIB="${PREFIX}/lib/libportaudio.dylib"
    install_name_tool -id ${LIB} ${LIB}
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
    ./configure --enable-static=yes \
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
    ./configure --enable-static=no \
        --enable-shared=yes \
        --disable-asm-optimizations \
        --disable-dependency-tracking \
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

    OPTIONS=""
    if [ ! $OSX_OLD ]; then
       OPTIONS='--enable-avx' 
    fi

    banner "Configure ${pkg} float"
    ./configure --enable-single \
        --prefix=${PREFIX} \
        --disable-fortran \
        --enable-sse \
        --enable-sse2 \
        --with-our-malloc \
        --enable-shared=yes \
        --enable-static=no $OPTIONS

    banner "Build ${pkg} float"
    make -j3

    banner "Install ${pkg} float"
    make install
}

function install_tcl() {
    export CFLAGS='-O2 -arch x86_64'
    export BUILD_STYLE='Release'
    export CONFIGURE_ARGS='--disable-symbols'
    pkg="TCL"
    cd "${CWD}"

    banner "${pkg}"
    rm -rf build/*
    rm -rf tcl-core*

    banner "Downloading ${pkg}"
    if [ ! -f "tcl-release.zip" ]
    then
        curl -L "https://github.com/tcltk/tcl/archive/core-8-6-6.zip" -o "tcl-release.zip"
    fi

    unzip -o  "tcl-release.zip"
    cd tcl-core*/unix

    banner "Build ${pkg}"
    #autoconf
    ./configure --prefix="/usr/local" --bindir="/usr/local/bin" --libdir="/Library/Frameworks" \
        --mandir="/usr/local/man" --enable-threads --enable-framework
    make

    banner "Installing ${pkg}"
    sudo make install
}

function install_iconv() {
    pkg="iconv"
    cd "${CWD}"

    banner "${pkg}"
    rm -rf libiconv-*
    brew unpack libiconv
    cd libiconv-*

    banner "Configure ${pkg}"
    ./configure --prefix=${PREFIX} \
        --enable-static=yes \
        --enable-shared=no

    banner "Build ${pkg}"
    make

    banner "Install ${pkg}"
    make install
}

function install_libffi() {
    pkg="libffi"
    cd "${CWD}"

    banner "${pkg}"
    rm -rf libffi-*
    brew unpack libffi
    cd libffi-*

    banner "Configure ${pkg}"
    ./configure --prefix=${PREFIX} \
        --enable-portable-binary \
        --enable-static=yes \
        --enable-shared=no

    banner "Build ${pkg}"
    make

    banner "Install ${pkg}"
    make install
}

function install_pcre() {
    pkg="pcre"
    cd "${CWD}"

    banner "${pkg}"
    rm -rf pcre-*
    brew unpack pcre
    cd pcre-*

    banner "Configure ${pkg}"
    ./configure --prefix=${PREFIX} \
        --disable-cpp \
        --enable-utf \
        --enable-unicode-properties

    banner "Build ${pkg}"
    make

    banner "Install ${pkg}"
    make install
}

function install_glib() {
    pkg="glib"
    cd "${CWD}"

    banner "${pkg}"
    rm -rf glib-*
    brew unpack glib
    cd glib-*

    wget https://raw.githubusercontent.com/macports/macports-ports/master/devel/glib2/files/universal.patch
    wget https://raw.githubusercontent.com/macports/macports-ports/master/devel/glib2/files/config.h.ed
    patch -i universal.patch -p0

    CPPFLAGS=-I/usr/local/Cellar/gettext/0.18.1.1/include

    banner "Configure ${pkg}"
    autoreconf
    ./configure --prefix=${PREFIX} \
        --enable-static=yes \
        --enable-shared=no \
        --disable-debug \
        --disable-option-checking \
        --with-pc-path=${PKG_CONFIG_PATH} \
        --with-internal-glib \
        LDFLAGS="-L${PREFIX}/lib" \
        CPPFLAGS="-I${PREFIX}/include -isystem ${PREFIX}/include"

    ed - config.h < config.h.ed

    banner "Build ${pkg}"
    make

    banner "Install ${pkg}"
    make install
}

function install_fluid() {
    pkg="fluidsynth"
    cd "${CWD}"

    banner "${pkg}"
    if [ ! -d fluidsynth ]
    then
        git clone --depth 1 https://github.com/FluidSynth/fluidsynth.git
    fi
    cd fluidsynth

    banner "Configure ${pkg}"
    rm -rf build
    mkdir build
    cd build
    cmake -DCMAKE_INSTALL_PREFIX=${PREFIX} \
        -DBUILD_SHARED_LIBS=OFF \
        -DCMAKE_C_FLAGS="${CFLAGS}" \
        -DCMAKE_EXE_LINKER_FLAGS="-liconv" \
        -DCMAKE_BUILD_TYPE=Release \
        -Denable-aufile=OFF \
        -Denable-floats=ON \
        -Denable-ipv6=OFF \
        -Denable-jack=OFF \
        -Denable-ladspa=OFF \
        -Denable-libsndfile=OFF \
        -Denable-oss=OFF \
        -Denable-coreaudio=OFF \
        -Denable-readline=OFF \
        -Denable-framework=OFF \
        ..

    banner "Build ${pkg}"
    make -j2

    banner "Install ${pkg}"
    make install

}

function install_gettext() {
    pkg="gettext"
    cd "${CWD}"

    banner "${pkg}"
    rm -rf gettext-*
    brew unpack gettext
    cd gettext-*

    banner "Configure ${pkg}"
    ./configure --prefix=${PREFIX} \
        --enable-static=yes \
        --enable-shared=yes \
        --disable-java \
        --disable-native-java  \
        --disable-curses \
        --disable-openmp \
        --disable-c++

    banner "Build ${pkg}"
    make

    banner "Install ${pkg}"
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
    ./configure  --with-tclsh="/usr/local/bin/tclsh8.6" \
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
    banner "Installing ${pkg}"
    make install
}


function install_tk() {
    export CFLAGS='-O2 -arch x86_64'
    pkg="TK"
    cd "${CWD}"

    banner "${pkg}"
    rm -rf build/*
    rm -rf tk-core*

    banner "Downloading ${pkg}"
    if [ ! -f "tk-release.zip" ]
    then
        wget "https://github.com/tcltk/tk/archive/core_8_6_6.zip" -O tk-release.zip
    fi

    unzip -o  "tk-release.zip"

    cd tk-core*/macosx

    echo "Patch tkMacOSXXStubs.c"
    echo "933,934c933,934
< 	    assert(bytes_per_row == 4 * scaled_width);
< 	    assert([bitmap_rep bytesPerPlane] == bytes_per_row * scaled_height);
---
> 	    //assert(bytes_per_row == 4 * scaled_width);
> 	    //assert([bitmap_rep bytesPerPlane] == bytes_per_row * scaled_height);" | patch tkMacOSXXStubs.c

    cd ../unix

    banner "Configure ${pkg}"
    ./configure --prefix="/usr/local" --bindir="/usr/local/bin" --libdir="/Library/Frameworks" \
    --mandir="/usr/local/man" --enable-threads --enable-framework \
    --with-tcl="/Library/Frameworks/Tcl.framework" \
    --enable-symbols --enable-aqua


    banner "Build ${pkg}"
    make

    banner "Installing ${pkg}"
    sudo make install
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
    glib)
        install_glib
        ;;
    ffi)
        install_libffi
        ;;
    gettext)
        install_gettext
        ;;
    pcre)
        install_pcre
        ;;
    fluid)
        install_fluid
        ;;
    iconv)
        install_iconv
        ;;
    *)
        echo "Choose from following: glib, gettext, pcre, modplug, ffi, fftw3, tcl, iconv, tcllib, tk, tklib, ogg, vorbis, flac, sndfile, portaudio or all"
        exit 1
        ;;
esac
