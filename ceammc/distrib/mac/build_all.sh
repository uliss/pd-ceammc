#!/bin/sh

rm -rf ../build

function build {
    echo "Building $1 ..." 
    $SHELL ./build_$1.sh > /dev/null
}

build fftw
build ogg
build opus
build speex
build flac
build vorbis
build sndfile
build portaudio

