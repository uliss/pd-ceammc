#!/bin/bash

sudo port install libsndfile \
	portaudio \
	fftw-3-single \
	libmad \
	luajit \
	rubberband \
	llvm-10 \
	cmake \
	ninja \
	glib2 \
	boost \
	ragel \
	gettext \
	pkgconfig \
	llvm_select \
	armadillo \
	scons \
	py310-pip \ 
	liblo

sudo port select llvm-10
sudo port select --set pip pip310
sudo port select --set pip3 pip310
pip3 install pddoc
