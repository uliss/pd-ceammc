#!/bin/bash

echo Architecture: $MSYS2_ARCH
echo Platform: %PLATFORM%
echo MSYS2 directory: %MSYS2_DIR%
echo MSYS2 system: %MSYSTEM%
echo Double precision: %DOUBLE_PRECISION%

pacman -Sy --needed --noconfirm pacman-mirrors
pacman -Syu --noconfirm

pacman -Sy --needed --noconfirm mingw-w64-${MSYS2_ARCH}-{binutils,crt,tools-git,compiler-rt} git make patch
pacman -Sy --needed --noconfirm mingw-w64-${MSYS2_ARCH}-{cmake,ninja,glib2,libmodplug,tcllib,tcl,tk,tklib,portaudio,fftw,libsndfile,boost,dlfcn,armadillo,llvm,polly,luajit}

