#!/bin/bash

# Text color variables
red=$(tput setaf 1) # red
blu=$(tput setaf 4) # blue
rst=$(tput sgr0)    # reset

if [ $# -ne 1 ]; then
	echo "${red}Usage: $(basename $0) APP_BUNDLE${rst}"
	exit 1
fi

BUNDLE=$1

if [ ! -d $BUNDLE ]; then
	echo "${red}Invalid bundle directory: $BUNDLE${rst}"
	exit 1
fi

find $BUNDLE -name *.d_amd64 -o -name *.dylib | while read external
do
	short_name=$(basename $external)
    dep=$(otool -L $external | grep -v -e '/System/Library' \
            -e '@loader_path' \
            -e $short_name \
            -e 'libc++' \
            -e 'libSystem' \
            -e 'libgcc')

	if [ "$dep" ]; then
		echo "${red}WARNING:${rst} external ${blu}${short_name}${rst} has this dependency:"
		echo "         $dep"
	fi
done


