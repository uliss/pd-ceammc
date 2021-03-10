#!/bin/bash

usage() {
    echo "Usage: $(basename $0) (-jpg|-png|-pdf) FILE"
}

if [ $# -ne 2 ]
then
    usage
    exit 1
fi

FMT=$1

case $1 in
-jpg) FMT=jpg;;
-png) FMT=png;;
-pdf) FMT=pdf;;
*)    echo "invalid format option: \"$1\", expected: -jpg, -png or -pdf"
      usage
      exit 1;;
esac

IN="$2"
PDF_OUT="${IN%.ps}.pdf"
PNG_OUT="${IN%.ps}.png"
JPEG_OUT="${IN%.ps}.jpg"

gsed -i \
    -e 's/Courier/Monaco/' \
    -e 's/findfont 12/findfont 11/' \
    -e 's/] 16 -0.0 0.0 0 false DrawText/] 16 -0.00 -0.3 0 false DrawText/' \
    "$IN"

pstopdf $IN $PDF_OUT

case $FMT in
jpg)
    sips -s format jpeg $PDF_OUT --out $JPEG_OUT
    rm -f $PDF_OUT
    ;;
png)
    sips -s format png $PDF_OUT --out $PNG_OUT
    rm -f $PDF_OUT
    ;;
esac
