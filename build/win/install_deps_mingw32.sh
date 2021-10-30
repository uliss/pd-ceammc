#!/bin/bash

ARCH=i686

# update pacman
#pacman -S --needed --noconfirm pacman-mirrors
pacman -Syu --noconfirm

# build tools
pacman -S --needed --noconfirm mingw-w64-${ARCH}-toolchain git make patch
# dependencies
pacman -S --needed --noconfirm mingw-w64-${ARCH}-{cmake,ninja,glib2,libmodplug,tcllib,tcl,tk,tklib,portaudio,fftw,libsndfile,boost,dlfcn,armadillo,llvm,polly,rubberband}

