#!/bin/bash

if [[ ${CEAMMC_BUILD_TYPE} == "Release" ]]
then 
	echo "Building DEB package..."
	cmake --build . --target  package
	ls *.deb
else 
	echo "Skip DEB package build..."
fi

