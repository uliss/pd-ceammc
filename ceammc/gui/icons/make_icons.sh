#!/bin/bash

INK='@INKSCAPE@'
ICONSET='@CMAKE_CURRENT_SOURCE_DIR@/pd_ceammc.iconset'

if [ -z "$1" ]
then
	echo "SVG file needed."
	exit;
fi

mkdir -p ${ICONSET}

BASE=${ICONSET}/icon
SVG="`pwd`/$1"

make_icon() {
    sz=$1
    sz2=$((${sz} * 2))
    $INK -z -D -e "${BASE}_${sz}x${sz}.png"    -f $SVG -w ${sz} -h ${sz}
    $INK -z -D -e "${BASE}_${sz}x${sz}@2x.png" -f $SVG -w ${sz2} -h ${sz2}
}

make_icon 16
make_icon 32
make_icon 128
make_icon 256
make_icon 512

cp ${ICONSET}/icon_512x512.png ./puredata-ceammc.png

if [ -e "@ICONUTIL@" ]
then
    echo "Making ICNS..."
    @ICONUTIL@ -c icns "${ICONSET}"
fi


if [ -e "@MAGICK_CONVERT@" ]
then
    echo "Making ICO..."
    PNGS=$(find "${ICONSET}" -name '*.png' | grep -v '@')
    @MAGICK_CONVERT@ ${PNGS} pd_ceammc.ico
fi
