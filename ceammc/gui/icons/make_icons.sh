#!/bin/bash

INK='/usr/local/bin/inkscape'
ICONSET='./pd_ceammc.iconset'

if [[ -z "$1" ]] 
then
	echo "SVG file needed."
	exit;
fi

mkdir -p ${ICONSET}

BASE=`pwd`/${ICONSET}/icon
SVG="`pwd`/$1"

function make_icon() {
    sz=$1
    sz2=$(($sz * 2))
    $INK -z -D -e "${BASE}_${sz}x${sz}.png"    -f $SVG -w ${sz} -h ${sz}
    $INK -z -D -e "${BASE}_${sz}x${sz}@2x.png" -f $SVG -w ${sz2} -h ${sz2}
}

make_icon 16
make_icon 32
make_icon 128
make_icon 256
make_icon 512

echo "Making ICNS..."
iconutil -c icns "${ICONSET}"

echo "Making ICO..."
PNGS=$(find "${ICONSET}" -name '*.png' | grep -v '@')
echo $PNGS
convert ${PNGS} pd_ceammc.ico



