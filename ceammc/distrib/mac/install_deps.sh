#!/bin/bash

if ! command -v rustup &> /dev/null
then
    echo "rustup could not be found ..."
    echo "you should install rust for build"
    echo "run: curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh"
    exit 1
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
        llvm-10 \
        llvm_select \
        luajit \
        ninja \
        pkgconfig \
        portaudio \
        py310-pip \
        ragel \
        rubberband \
        scons

sudo port select llvm llvm-10
sudo port select --set pip pip310
sudo port select --set pip3 pip310
pip3 install pddoc
