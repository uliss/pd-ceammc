#!/bin/bash

if [ $# -ne 1 ]
then
    echo "Usage: $0 mod_name"
    exit 1
fi

MOD=$1
MOD_DEFINE="MOD_$(echo $MOD | tr '[:lower:]' '[:upper:]')_H"
MOD_H="$MOD/mod_${MOD}.h"
MOD_CPP="$MOD/mod_$MOD.cpp"

if [ ! -d $MOD ]
then
    echo "Directory $MOD is not exists."
    exit 1
fi

function trim() {
    echo $* | sed -e 's/^[[:space:]]*//' -e 's/[[:space:]]*$//';
}

function find_cpp_files() {
    find $MOD -name \*.cpp -not -path $MOD_CPP | xargs cat | grep 'extern \"C\" ' | grep setup | sort
}

function find_c_files() {
    find $MOD -name \*.c -not -path $MOD_CPP | xargs cat | grep setup_ | sort
}

function generate_header() {
    echo "#ifndef ${MOD_DEFINE}
#define ${MOD_DEFINE}

void ceammc_${MOD}_setup();

#endif" > "$MOD_H"

    echo "#include \"mod_$MOD.h\"
" > "$MOD_CPP"
}

generate_header

find_cpp_files | while read line
do
    line="$(trim $line);"
    echo $line >> "$MOD_CPP"
done

find_c_files | while read line
do
    line="extern \"C\" $(trim $line);"
    echo $line >> "$MOD_CPP"
done

echo "
void ceammc_${MOD}_setup() {" >> $MOD_CPP

find_cpp_files | while read line
do
    line="$(trim $line);"
    line=$(echo "$line" | cut -c 16-)
    echo "    $line" >> "$MOD_CPP"
done

find_c_files | while read line
do
    line="$(trim $line);"
    line=$(echo "$line" | cut -c 5-)
    echo "    $line" >> "$MOD_CPP"
done

echo "}" >> "$MOD_CPP"

