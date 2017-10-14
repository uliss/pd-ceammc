#!/bin/bash


# Text color variables
red=$(tput setaf 1) #  red
blu=$(tput setaf 4) #  blue
rst=$(tput sgr0)             # Reset

if [ $# -ne 1 ]; then
	echo "${red}Usage: $(basename $0) APP_BUNDLE${rst}"
	exit 1
fi

BUNDLE=$1

if [ ! -d $BUNDLE ]; then
	echo "${red}Invalid bundle directory: $BUNDLE${rst}"
	exit 1
fi

find $BUNDLE -name *.d_fat | while read external
do
	short_name=$(basename $external)
	dep=`otool -L $external | grep -v '/System/Library' | grep -v '@loader_path' | grep -v $short_name | grep -v libc++ | grep -v libSystem`
	if [ "$dep" ]; then
		echo "${red}WARNING:${rst} external ${blu}${short_name}${rst} has this dependency:"
		echo "         $dep"
	fi
done


