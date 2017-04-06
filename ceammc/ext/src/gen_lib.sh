#!/bin/sh

if [ $# -ne 1 ]
then
    echo "Usage: $0 mod_name"
    exit 1
fi

MOD=$1
MOD_CPP="$MOD/mod_$MOD.cpp"

if [ ! -d $MOD ]
then
    echo "Directory $MOD is not exists."
    exit 1
fi

echo "#include \"mod_$MOD.h\"
" > "$MOD_CPP"

mod_ext=""
mod_call=""

find $MOD -name \*.cpp | xargs cat | grep 'extern \"C\" ' | grep setup | sort | while read line
do
    echo "$line;" >> "$MOD_CPP"
done

echo "
void ceammc_list_setup() {" >> $MOD_CPP

find $MOD -name \*.cpp | xargs cat | grep 'extern \"C\" ' | grep setup | sort | while read line
do
    l=`echo "$line" | cut -c 16-`
    echo "    $l;" >> "$MOD_CPP"
done

echo "}" >> "$MOD_CPP"

