#!/bin/bash

echo Architecture: $MINGW_CHOST

#pacman -Sy --needed --noconfirm pacman-mirrors
#pacman -Syu --noconfirm

pacman -Sy --needed --noconfirm ${MINGW_PACKAGE_PREFIX}-{binutils,crt,tools-git,compiler-rt} git make patch
pacman -Sy --needed --noconfirm ${MINGW_PACKAGE_PREFIX}-{cmake,ninja,glib2,libmodplug,tcllib,tcl,tk,tklib,portaudio,fftw,libsndfile,boost,dlfcn,armadillo,llvm,polly,luajit}

