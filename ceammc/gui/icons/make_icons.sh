#!/bin/bash

INK='@INKSCAPE@'
ICONSET='@CMAKE_CURRENT_SOURCE_DIR@/pd_ceammc.iconset'
ICONSET_WIN='@CMAKE_CURRENT_SOURCE_DIR@/pd_ceammc_win.iconset'
DOC_ICONSET='@CMAKE_CURRENT_SOURCE_DIR@/pd_doc.iconset'
MAC_DOC_ICON_TEMPLATE='/System/Library/CoreServices/CoreTypes.bundle/Contents/Resources/GenericDocumentIcon.icns'

if [ -z "$1" ]
then
	echo "SVG file needed."
	exit;
fi

mkdir -p ${ICONSET}
rm -rf ${ICONSET_WIN}
mkdir -p ${ICONSET_WIN}

BASE=${ICONSET}/icon
SVG="`pwd`/$1"

make_icon() {
    sz=$1
    sz2=$((${sz} * 2))
    $INK --export-area-drawing --export-margin=0 --export-filename="${BASE}_${sz}x${sz}.png" \
        -w ${sz} -h ${sz} $SVG
    $INK --export-area-drawing --export-margin=0 --export-filename="${BASE}_${sz}x${sz}@2x.png" \
        -w ${sz2} -h ${sz2} $SVG
}

make_win_icon() {
    svg=$1
    sz=$2
    $INK --export-area-page --export-filename="${ICONSET_WIN}/icon_${sz}x${sz}.png" \
        -w ${sz} -h ${sz} `pwd`/$svg
}

make_icon 16
make_icon 32
make_icon 128
make_icon 256
make_icon 512
make_win_icon pd_ceammc_win_16.svg 16
make_win_icon pd_ceammc_win_32.svg 32
make_win_icon pd_ceammc_win_48.svg 48
make_win_icon pd_ceammc_win_64.svg 64
make_win_icon pd_ceammc_win_48.svg 96
make_win_icon pd_ceammc_win_64.svg 256

cp ${ICONSET}/icon_512x512.png ./puredata-ceammc.png
cp ${ICONSET_WIN}/icon_256x256.png ./puredata-ceammc_win.png

if [ -e "@ICONUTIL@" ]
then
    echo "Making ICNS..."
    @ICONUTIL@ -c icns "${ICONSET}"

    if [ -f ${MAC_DOC_ICON_TEMPLATE} ]; then
        echo "Generating MacOSX doc iconset..."
        @ICONUTIL@ -c iconset ${MAC_DOC_ICON_TEMPLATE} -o ${DOC_ICONSET}
        cd ${DOC_ICONSET}
        for i in *.png
        do
            echo "generating doc icon: $i"
            overlay="${ICONSET}/$(basename $i)"
            convert $i \( ${overlay} -resize 50% \) -gravity center -geometry +0+0 -composite ${i}
        done
        cd ..

        @ICONUTIL@ -c icns ${DOC_ICONSET}
    fi
fi

if [ -e "@MAGICK_CONVERT@" ]
then
    echo "Making ICO..."
    PNGS=$(find "${ICONSET}" -name '*.png' | grep -v '@')
    @MAGICK_CONVERT@ ${PNGS} pd_ceammc.ico

    PNGS=$(find "${ICONSET_WIN}" -name '*.png')
    @MAGICK_CONVERT@ ${PNGS} pd_ceammc_win.ico

    PNGS=$(find "${DOC_ICONSET}" -name '*.png')
    @MAGICK_CONVERT@ ${PNGS} pd_doc_win.ico
fi
