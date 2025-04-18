#!/bin/sh

# Text color variables
red=$(tput setaf 1) # red
green=$(tput setaf 2) # green
blu=$(tput setaf 4) # blue
rst=$(tput sgr0)    # reset

if [ $# -ne 1 ]; then
    echo "${red}Usage: $(basename $0) LIBRARY_DIR${rst}"
    exit 1
fi

CEAMMC=$1

if [ ! -d $CEAMMC ]; then
    echo "${red}Invalid library directory:${rst} $CEAMMC"
    exit 1
fi

num_errors=0

for external in $(find $CEAMMC -name *.d_amd64 -o -name *.d_fat -o -name *.pd_darwin -o -name *.dylib)
do
    short_name=$(basename $external)

    dep=$(otool -L $external | grep -v -e '/System/Library' \
            -e '@loader_path' \
            -e '@rpath' \
            -e $short_name \
            -e 'libc++' \
            -e '/usr/lib' \
            -e 'libSystem' \
            -e 'libgcc')

    if [ "$dep" ]; then
        echo "${red}WARNING:${rst} external ${blu}${short_name}${rst} has this dependencies:"
        echo "         $dep"
        let num_errors=num_errors+1

        printf "check %-30s ❌\n" "'${short_name}'"
    else
        printf "check %-30s ✅\n" "'${short_name}'"
    fi
done

if [[ $num_errors -eq 0 ]]
then
    echo "Check library: ${green}OK${rst}"
    exit 0
else
    echo "Check library: ${red}FAILED${rst}"
    exit 2
fi
