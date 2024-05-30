#!/bin/bash

if ! command -v rustup &> /dev/null
then
    echo "rustup could not be found ..."
    echo "you should install rust for build"
    echo "run: curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh"
    exit 1
fi

rustup update

sudo port selfupdate
sudo port upgrade outdated
sudo port uninstall inactive

if [[ $(uname -m) == "arm64" ]]
then
    llvm_ver=17
else
    llvm_ver=10
fi

sudo port install libsndfile \
        armadillo \
        boost \
        cairo \
        cmake \
        fftw-3-single \
        gettext \
        glib2 \
        liblo \
        libltc \
        libmad \
        libmagic \
        libmodplug \
        libsamplerate\
        libsndfile \
        llvm-${llvm_ver} \
        llvm_select \
        luajit \
        ninja \
        pkgconfig \
        portaudio \
        py310-pip \
        ragel \
        rubberband \
        scons

sudo port select llvm mp-llvm-${llvm_ver}
pip3.11 install pddoc
